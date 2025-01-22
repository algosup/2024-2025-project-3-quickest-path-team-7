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

struct Node {
    int id;
    int weight;
    int estimated_cost;

    // Make the first Node in a priority queue the one with the smallest estimated distance
    bool operator<(const Node& other) const {
        return estimated_cost > other.estimated_cost;
    }
};

void printPQ(priority_queue<Node> pq) {
    cout << "\nPriority Queue: " << endl;
    while (!pq.empty()) {
        Node top = pq.top();
        cout << "estim: " << top.estimated_cost << "            id: " << top.id << endl;
        pq.pop();
    }
    cout << endl;
}

void reconstruct_path(vector<int_pair>& node_before, Path& path_data) {
    int current_node = path_data.end;
    while (current_node != path_data.start) {
        path_data.path.insert(path_data.path.begin(), {current_node, node_before[current_node].second});
        path_data.distance += node_before[current_node].second;
        current_node = node_before[current_node].first;
    }
    path_data.path.insert(path_data.path.begin(), {current_node, 0});
}

// A* derived algorithm to find the shortest path between two nodes
void find_path(Graph& graph, Path& path_data) {

    int n = graph.map.size();

    vector<int>         cost_from_start(n, INF);
    vector<int_pair>    node_before(n);
    vector<bool>        checked(n, false);
    // Min-heap priority queue: (estimated distance, {node, weight})
    priority_queue<Node> pq;

    int start_to_end_estimation = estimate_distance(graph, path_data.start, path_data.end);
    path_data.estimated_distance = start_to_end_estimation;

    // Fill the first node data
    Node current_node;
    current_node.id = path_data.start;
    current_node.weight = 0;
    current_node.estimated_cost = start_to_end_estimation;

    // Push the first node to the priority queue
    pq.push(current_node);

    // Update the costs
    cost_from_start[current_node.id] = current_node.weight;
    
    while (!pq.empty()) {

        // Load the node with the smallest estimated cost
        Node current_node = pq.top();

        // Check if we reached the destination
        if(current_node.id == path_data.end) {
            reconstruct_path(node_before, path_data);
            return;
        }

        // Remove the node from the priority queue
        pq.pop();
        // Prevent revisiting the node
        checked[current_node.id] = true;

        // For each neighbor of the current node
        for (int_pair node : graph.map[current_node.id]) {

            Node neighbor;
            neighbor.id = node.first;
            neighbor.weight = node.second;

            // We measure the cost from the start to the neighbor through this current node
            int local_cost_from_start = cost_from_start[current_node.id] + neighbor.weight;
            
            // If this measure is better than the previous one, we update the better path to this node, thus the costs
            if(cost_from_start[neighbor.id] > local_cost_from_start) {
                // If not already checked, (thus already in the priority queue)
                if (!checked[neighbor.id]) {
                    // Update the node access
                    node_before[neighbor.id] = {current_node.id, neighbor.weight};
                    // Update the costs
                    cost_from_start[neighbor.id] = local_cost_from_start;
                    int estimated_distance_to_end = estimate_distance(graph, neighbor.id, path_data.end);
                    neighbor.estimated_cost = local_cost_from_start + estimated_distance_to_end;
                    // And finally, push the neighbor to the priority queue
                    pq.push(neighbor);
                }
            }
        }
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