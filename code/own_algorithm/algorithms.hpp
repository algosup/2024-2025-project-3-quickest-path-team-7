#ifndef ALGORITHM_HPP
#define ALGORITHM_HPP

#include "header.hpp"

// Function to estimate the distance between two nodes using landmarks
int estimate_distance(Graph& graph, int source, int destination) {
    int estimation = 0;
    // For each landmark : "
    for (int i = 0; i < graph.landmarks.size(); i++) {
        // Take the absolute value of the difference between these two distances
        int diff = abs(graph.landmark_distance[source][i] - graph.landmark_distance[destination][i]);
        // Keep only the maximum of these estimations
        if (diff > estimation) {
            estimation = diff;
        }
    }
    return estimation;
}

// Function to reconstruct the path from the forward and backward visited nodes
void reconstruct_bidirectional_path(Astar& astar1, Astar& astar2, Path& path_data) {
    // Reconstruct path from start to meeting node
    vector<int_pair> forward_path;
    int current_node = meeting_node;
    while (current_node != -1) {
        forward_path.insert(forward_path.begin(), {current_node, astar1.node_before[current_node].second});
        current_node = astar1.node_before[current_node].first;
    }

    // Reconstruct path from meeting node to end
    vector<int_pair> backward_path;
    current_node = meeting_node;
    while (current_node != -1) {
        current_node = astar2.node_before[current_node].first;
        if (current_node != -1) { // Skip the meeting node itself
            backward_path.push_back({current_node, astar2.node_before[current_node].second});
        }
    }

    // Combine forward and backward paths
    path_data.path = forward_path;
    path_data.path.insert(path_data.path.end(), backward_path.begin(), backward_path.end());
}

// Function for forward search (Thread 1)
void forward_search(Graph& graph, Astar& astar1, int end) {
    bool push = false;
    bool brother_visited = false;
    while (!astar1.pq.empty() && !meeting_found.load()) {
        
        Node current = astar1.pq.top();
        astar1.pq.pop();
        astar1.iterations++;

        // Early termination: If this node is visited by backward search
        {
            lock_guard<mutex> lock(visited_mutex);
            brother_visited = visited_backward[current.id];
        }
        if (brother_visited) {
            meeting_node.store(current.id);
            meeting_found.store(true);
            return;
        }

        // Explore neighbors
        for (int i = graph.adjacency_start[current.id]; i < graph.adjacency_start[current.id+1]; i++) {
            Edge neighbor = graph.edges[i];
            int local_cost_from_start = astar1.cost_from_start[current.id] + neighbor.weight;

            // Update visited set and push to priority queue
            if (local_cost_from_start < astar1.cost_from_start[neighbor.id]){
                {
                    lock_guard<mutex> lock(visited_mutex);
                    if (!visited_forward[neighbor.id]) {
                        // Mark node as visited
                        visited_forward[neighbor.id] = true;
                        push = true;
                    }
                } 
                if (push) {
                    // Update node_before and cost_from_start
                    astar1.node_before[neighbor.id] = {current.id, neighbor.weight};
                    astar1.cost_from_start[neighbor.id] = local_cost_from_start;
                    astar1.pq.push({neighbor.id, neighbor.weight, local_cost_from_start + estimate_distance(graph, neighbor.id, end)});
                }
            }
        }
    }
}

// Function for backward search (Thread 2)
void backward_search(Graph& graph, Astar& astar2, int start) {
    bool push = false;
    bool brother_visited = false;
    while (!astar2.pq.empty() && !meeting_found.load()) {
        
        Node current = astar2.pq.top();
        astar2.pq.pop();
        astar2.iterations++;

        // Early termination: If this node is visited by forward search
        {
            lock_guard<mutex> lock(visited_mutex);
            brother_visited = visited_forward[current.id];
        }
        if (brother_visited) {
            meeting_node.store(current.id);
            meeting_found.store(true);
            return;
        }

        // Explore neighbors
        for (int i = graph.adjacency_start[current.id]; i < graph.adjacency_start[current.id+1]; i++) {
            Edge neighbor = graph.edges[i];
            int local_cost_from_start = astar2.cost_from_start[current.id] + neighbor.weight;

            // Update visited set and push to priority queue
            if (local_cost_from_start < astar2.cost_from_start[neighbor.id]){
                {
                    lock_guard<mutex> lock(visited_mutex);
                    if (!visited_backward[neighbor.id]) {
                        // Mark node as visited
                        visited_backward[neighbor.id] = true;
                        push = true;
                    }
                } 
                if (push) {
                    // Update node_before and cost_from_start
                    astar2.node_before[neighbor.id] = {current.id, neighbor.weight};
                    astar2.cost_from_start[neighbor.id] = local_cost_from_start;
                    astar2.pq.push({neighbor.id, neighbor.weight, local_cost_from_start + estimate_distance(graph, neighbor.id, start)});
                }
            }
        }
    }
}

// Main function to find the shortest path using bidirectional A*
void find_path(Graph& graph, Path& path_data, Astar& astar1, Astar& astar2) {

    path_data.estimated_distance = estimate_distance(graph, path_data.start, path_data.end);;
    
    // Initialize priority queues
    astar1.pq.push({path_data.start, 0, path_data.estimated_distance});
    astar2.pq.push({path_data.end, 0, path_data.estimated_distance});

    //path_data.estimated_distance /= heuristic_weight;

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

    path_data.distance = astar1.cost_from_start[meeting_node] + astar2.cost_from_start[meeting_node];
    
    if (meeting_node != 0) {
        // Reconstruct the path
        reconstruct_bidirectional_path(astar1, astar2, path_data);
    } else {
        // No path found
        path_data.path.clear();
        reset_algorithm_data(graph, path_data, astar1, astar2);      }
}


// Function to return the list of all the shortest paths from the source node to every other node. So distances[i] will contain the shortest distance from the source node to node i
vector<int> shortestPaths(Graph& graph, int source) {
   /*  int n = graph.map_size; // Number of nodes in the graph
    vector<int> distances(n, INF); // Initialize distances to infinity
    distances[source] = 0; // Distance to the source node is 0

    // Min-heap priority queue: (distance, node)
    priority_queue<int_pair, vector<int_pair>, greater<>> pq;

    // Preload the source node's neighbors
    for (int i = graph.adjacency_start[source]; i <= graph.adjacency_start[source+1]; i++) {
        Edge neighbor = graph.edges[i];
        distances[neighbor.id] = neighbor.weight;
        pq.push({neighbor.weight, neighbor.id});
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

        for (int i = graph.adjacency_start[currentNode]; i <= graph.adjacency_start[currentNode+1]; i++) {
            Edge neighbor = graph.edges[i];
            int newDist = currentDist + neighbor.weight;
            if (newDist < distances[neighbor.id]) {
                distances[neighbor.id] = newDist;
                pq.push({newDist, neighbor.id});
            }
        }
    }
*/
    return {1,3,4};//distances; 
}

#endif