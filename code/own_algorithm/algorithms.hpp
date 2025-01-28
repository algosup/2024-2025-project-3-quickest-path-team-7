#ifndef ALGORITHM_HPP
#define ALGORITHM_HPP

#include "header.hpp"

// Function to estimate the distance between two nodes using landmarks
int estimate_distance(Graph& graph, int source, int destination) {
    int estimation = 0;
    // For each landmark : "
    for (int i = 0; i < graph.landmarks.size(); i++) {
        int landmark = graph.landmarks[i];
        // Get the distance between the source and the landmark
        int source_distance = graph.landmark_distance[source][i];
        // Get the distance between the destination and the landmark
        int destination_distance = graph.landmark_distance[destination][i];
        // Take the absolute value of the difference between these two distances
        int diff = abs(source_distance - destination_distance);
        // Keep only the maximum of these estimations
        estimation = max(estimation, diff);
    }
    return estimation;
}

// Function to reconstruct the path from the forward and backward visited nodes
void reconstruct_bidirectional_path(const vector<int_pair>& node_before_forward, 
                                    const vector<int_pair>& node_before_backward, 
                                    int meeting_node, 
                                    Path& path_data) {
    // Reconstruct path from start to meeting node
    vector<int_pair> forward_path;
    int current_node = meeting_node;
    while (current_node != -1) {
        forward_path.push_back({current_node, node_before_forward[current_node].second});
        current_node = node_before_forward[current_node].first;
    }
    reverse(forward_path.begin(), forward_path.end());

    // Reconstruct path from meeting node to end
    vector<int_pair> backward_path;
    current_node = meeting_node;
    while (current_node != -1) {
        current_node = node_before_backward[current_node].first;
        if (current_node != -1) { // Skip the meeting node itself
            backward_path.push_back({current_node, node_before_backward[current_node].second});
        }
    }

    // Combine forward and backward paths
    path_data.path = forward_path;
    path_data.path.insert(path_data.path.end(), backward_path.begin(), backward_path.end());
}

// Function for forward search (Thread 1)
void forward_search(Graph& graph, Astar& astar1, int end) {
    bool already_met = false;
    while (!astar1.pq.empty() && !already_met) {
        
        Node current = astar1.pq.top();
        astar1.pq.pop();
        astar1.iterations++;
        //cout << "forward current : " << current.id << endl;

        // Early termination: If this node is visited by backward search
        {
            lock_guard<mutex> lock(visited_mutex);
            if (visited_backward[current.id]) {
                best_path_cost.store({current.id, min(best_path_cost.load().second, current.estimated_cost)});
                {
                    lock_guard<mutex> lock2(meeting_mutex);
                    meeting_found = true;
                }                
                cout << "\n\n                                        Forward search found meeting node: " << current.id << endl;
                return;
            }
        }

        // Explore neighbors
        for (auto neighbor : graph.map[current.id]) {
            int neighbor_id = neighbor.first;
            int edge_cost = neighbor.second;

            int local_cost_from_start = astar1.cost_from_start[current.id] + edge_cost;

            // Update visited set and push to priority queue
            {
                lock_guard<mutex> lock(visited_mutex);
                if (!visited_forward[neighbor_id] || local_cost_from_start < astar1.cost_from_start[neighbor_id]) {
                    // Update node_before and cost_from_start
                    astar1.node_before[neighbor_id] = {current.id, edge_cost};
                    astar1.cost_from_start[neighbor_id] = local_cost_from_start;

                    // Mark node as visited
                    visited_forward[neighbor_id] = true;

                    // Add to priority queue
                    int estimated_distance_to_end = estimate_distance(graph, neighbor_id, end);
                    astar1.pq.push({neighbor_id, local_cost_from_start, local_cost_from_start + estimated_distance_to_end});
                }
            }
        }
        { 
            lock_guard<mutex> lock(meeting_mutex);
            already_met = meeting_found;
        }
    }
}

// Function for backward search (Thread 2)
void backward_search(Graph& graph, Astar& astar2, int start) {
    bool already_met = false;
    while (!astar2.pq.empty() && !already_met) {
        
        Node current = astar2.pq.top();
        astar2.pq.pop();
        astar2.iterations++;

        //cout << "backward current : " << current.id << endl;

        // Early termination: If this node is visited by forward search
        {
            lock_guard<mutex> lock(visited_mutex);
            if (visited_forward[current.id]) {
                best_path_cost.store({current.id, min(best_path_cost.load().second, current.estimated_cost)});
                {
                    lock_guard<mutex> lock2(meeting_mutex);
                    meeting_found = true;
                }
                cout << "\n\n                                        Backward search found meeting node: " << current.id << endl;
                return;
            }
        }

        // Explore neighbors
        for (auto neighbor : graph.map[current.id]) {
            int neighbor_id = neighbor.first;
            int edge_cost = neighbor.second;

            int local_cost_from_start = astar2.cost_from_start[current.id] + edge_cost;

            // Update visited set and push to priority queue
            {
                lock_guard<mutex> lock(visited_mutex);
                if (!visited_backward[neighbor_id] || local_cost_from_start < astar2.cost_from_start[neighbor_id]) {
                    // Update node_before and cost_from_start
                    astar2.node_before[neighbor_id] = {current.id, edge_cost};
                    astar2.cost_from_start[neighbor_id] = local_cost_from_start;

                    // Mark node as visited
                    visited_backward[neighbor_id] = true;

                    // Add to priority queue
                    int estimated_distance_to_start = estimate_distance(graph, neighbor_id, start);
                    astar2.pq.push({neighbor_id, local_cost_from_start, local_cost_from_start + estimated_distance_to_start});
                }
            }
        }
        { 
            lock_guard<mutex> lock(meeting_mutex);
            already_met = meeting_found;
        }
    }
}

// Main function to find the shortest path using bidirectional A*
void find_path(Graph& graph, Path& path_data, Astar& astar1, Astar& astar2) {

    int map_size = graph.map.size();

    int end_to_start_estimation = estimate_distance(graph, path_data.start, path_data.end);
    path_data.estimated_distance = end_to_start_estimation;
    end_to_start_estimation *= WEIGHT;

    // Initialize priority queues
    astar1.pq.push({path_data.start, 0, end_to_start_estimation});
    astar2.pq.push({path_data.end, 0, end_to_start_estimation});

    cout << "Starting bidirectional A* search from " << path_data.start << " to " << path_data.end << endl;

    // Initialize visited sets
    visited_forward[path_data.start] = true;
    visited_backward[path_data.end] = true;

    // Initialize node tracking
    astar1.cost_from_start[path_data.start] = 0;
    astar2.cost_from_start[path_data.end] = 0;

    // Launch threads for forward and backward search
    thread forward_thread(forward_search, ref(graph), ref(astar1), path_data.end);
    thread backward_thread(backward_search, ref(graph), ref(astar2), path_data.start);

    // Wait for both threads to finish
    forward_thread.join();
    backward_thread.join();

    cout << "Forward iterations: " << astar1.iterations << endl;
    cout << "Backward iterations: " << astar2.iterations << endl;
    cout << "Total iterations: " << astar1.iterations + astar2.iterations << endl;

    // Find the meeting node
    int meeting_node = -1;
    int min_cost = best_path_cost.load().second;
    path_data.distance = min_cost;

    for (size_t i = 0; i < map_size; ++i) {
        if (visited_forward[i] && visited_backward[i]) {
            int total_cost = astar1.cost_from_start[i] + astar2.cost_from_start[i];
            if (total_cost <= min_cost) {
                min_cost = total_cost;
                meeting_node = i;
            }
        }
    }

    if (meeting_node != -1) {
        // Reconstruct the path
        cout << "Meeting node found: " << meeting_node << " with cost: " << min_cost << endl;
        reconstruct_bidirectional_path(astar1.node_before, astar2.node_before, meeting_node, path_data);
        path_data.distance = min_cost;
    } else {
        // No path found
        cout << "No path found between " << path_data.start << " and " << path_data.end << endl;
        path_data.path.clear();
        path_data.distance = -1;
    }
}


// Function to return the list of all the shortest paths from the source node to every other node. So distances[i] will contain the shortest distance from the source node to node i
vector<int> shortestPaths(Graph& graph, int source) {
    int n = graph.map.size(); // Number of nodes in the graph
    vector<int> distances(n, numeric_limits<int>::max()); // Initialize distances to infinity
    distances[source] = 0; // Distance to the source node is 0

    // Min-heap priority queue: (distance, node)
    priority_queue<int_pair, vector<int_pair>, greater<>> pq;

    // Preload the source node's neighbors
    for (int_pair neighbor_weight_pair : graph.map[source]) {
    int neighbor = neighbor_weight_pair.first;
    int weight = neighbor_weight_pair.second;
    distances[neighbor] = weight;
    pq.push({weight, neighbor});
}

    pq.push({0, source});

    while (!pq.empty()) {
        int_pair top = pq.top();
        int currentDist = top.first;
        int currentNode = top.second;
        pq.pop();

        // If the popped distance is greater than the recorded distance, skip processing
        if (currentDist > distances[currentNode]) {
            continue;
        }

        // Relax all edges of the current node
        for (int_pair neighbor_weight_pair : graph.map[currentNode]) {
            int neighbor = neighbor_weight_pair.first;
            int weight = neighbor_weight_pair.second;
            int newDist = currentDist + weight;
            if (newDist < distances[neighbor]) {
                distances[neighbor] = newDist;
                pq.push({newDist, neighbor});
            }
        }
    }

    return distances;
}

#endif