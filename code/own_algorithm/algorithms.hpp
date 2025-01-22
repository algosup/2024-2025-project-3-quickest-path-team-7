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
    int estimated_distance;

    // Make the first Node in a priority queue the one with the smallest estimated distance
    bool operator<(const Node& other) const {
        return estimated_distance > other.estimated_distance;
    }
};

void printPQ(priority_queue<Node> pq) {
    cout << "\nPriority Queue: " << endl;
    while (!pq.empty()) {
        Node top = pq.top();
        cout << "estim: " << top.estimated_distance << "            id: " << top.id << endl;
        pq.pop();
    }
    cout << endl;
}

// A* derived algorithm to find the shortest path between two nodes
void find_path(Graph& graph, Path& path_data) {
    
    // Exclusion list TESTER UNE HASH TABLE
    vector<bool> visited(graph.map.size(), false);
    vector<bool> dead_end(graph.map.size(), false);
    bool all_visited = false;
    bool all_dead_end = false;

    Node current_node;
    current_node.id = path_data.start;
    current_node.weight = 0;
    current_node.estimated_distance = estimate_distance(graph, path_data.start, path_data.end);
    visited[current_node.id] = true;
    path_data.path.push_back({current_node.id, current_node.weight});

    cout << "Estimation: " << formatWithSpaces(current_node.estimated_distance) << endl;

    while (current_node.id != path_data.end) {

        // Min-heap priority queue: (estimated distance, node) to automatically sort the connected nodes of the current_node
        priority_queue<Node> pq;
        all_visited = true;
        for (int_pair node : graph.map[current_node.id]) {
            if (!visited[node.first]) { 
                all_visited = false;

                Node neighbor;
                neighbor.id = node.first;
                neighbor.weight = node.second;
                neighbor.estimated_distance = /* neighbor.weight + */ estimate_distance(graph, neighbor.id, path_data.end);

                pq.push(neighbor);
                visited[neighbor.id] = true;

            }
        }

        if (all_visited) {
            // So we go back to the last node and mark its other neighbors as unvisited
            dead_end[current_node.id] = true;
            path_data.path.pop_back();
            path_data.distance -= current_node.weight;
            current_node.id = path_data.path.back().first;
            for (int_pair node : graph.map[current_node.id]) {
                // Of course the current dead node is kept as visited
                if (!dead_end[node.first]) {
                    visited[node.first] = false;
                }
            }
            
            continue;
        }
        //printPQ(pq);

        current_node = pq.top();
        path_data.path.push_back({current_node.id, current_node.weight});
        path_data.distance += current_node.weight;

    }
    return;
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