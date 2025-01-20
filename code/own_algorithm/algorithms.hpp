#ifndef ALGORITHM_HPP
#define ALGORITHM_HPP

#include "header.hpp"

// Function to return the list of all the shortest paths from the source node to every other node. So distances[i] will contain the shortest distance from the source node to node i
vector<int> shortestPaths(Graph& graph, int source) {
    int n = graph.map.size(); // Number of nodes in the graph
    vector<int> distances(n, numeric_limits<int>::max()); // Initialize distances to infinity
    distances[source] = 0; // Distance to the source node is 0

    // Min-heap priority queue: (distance, node)
    priority_queue<int_pair, vector<int_pair>, greater<>> pq;

    // Preload the source node's neighbors
    for (const auto& [neighbor, weight] : graph.map[source]) {
        distances[neighbor] = weight;
        pq.push({weight, neighbor});
    }

    pq.push({0, source});

    while (!pq.empty()) {
        auto [currentDist, currentNode] = pq.top();
        pq.pop();

        // If the popped distance is greater than the recorded distance, skip processing
        if (currentDist > distances[currentNode]) {
            continue;
        }

        // Relax all edges of the current node
        for (const auto& [neighbor, weight] : graph.map[currentNode]) {
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