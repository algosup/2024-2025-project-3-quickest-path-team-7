#ifndef ALGORITHM_HPP
#define ALGORITHM_HPP

#include "header.hpp"

int estimate_distance(Graph& graph, int source, int destination) {
    cout << "Estimating distance between " << source << " and " << destination << "..." << endl;
    int estimation = 0;
    // For each landmark : "
    for (int i = 0; i < graph.landmarks.size(); i++) {
        int landmark = graph.landmarks[i];
        cout << "\n\nfor landmark " << landmark << " : " << endl;
        // Get the distance between the source and the landmark
        int source_distance = graph.landmark_distance[source][i];
        cout << "source distance : " << source_distance << endl;
        // Get the distance between the destination and the landmark
        int destination_distance = graph.landmark_distance[destination][i];
        cout << "destination distance : " << destination_distance << endl;
        // Take the absolute value of the difference between these two distances
        int diff = abs(source_distance - destination_distance);
        cout << "diff : " << diff << endl;
        // Keep only the maximum of these estimations
        estimation = max(estimation, diff);
    }
    cout << "estimated distance : " << estimation << endl;
    return estimation;

}

// A* algorithm to find the shortest path between two nodes
void find_path(Graph& graph, Path& path_data) {
    
    // Min-heap priority queue: (estimated distance, node)
    priority_queue<pair<int, int_pair>, vector<pair<int, int_pair>>, greater<>> pq;

    cout << "Estimating distance..." << endl;
    int estimation = estimate_distance(graph, path_data.start, path_data.end);
    int startDist = 0;
    pq.push({estimation, {startDist, path_data.start}});

    cout << "Calculating shortest path..." << endl;

    while (!pq.empty()) {
        pair<int, int_pair> top = pq.top();
        int estimatedDist = top.first;
        int currentNode = top.second.first;
        int currentDist = top.second.second;
        pq.pop();

        cout << "Current node: " << currentNode << " with distance: " << estimatedDist << endl;

        path_data.path.push_back({currentNode, currentDist});

        if (currentNode == path_data.end) {
            return;
        }

        for (int_pair node : graph.map[currentNode]) {
            int neighbor = node.first;
            int weight = node.second;
            int newEstimation = estimate_distance(graph, neighbor, path_data.end);
            pq.push({newEstimation, {neighbor, weight}});
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