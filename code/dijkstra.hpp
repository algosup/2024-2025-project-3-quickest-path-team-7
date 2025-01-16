#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include "header.hpp"

void dijkstra_algorithm(Graph& graph, PathData& path_data, bool break_early = false){

    int n = graph.start_indices.size() - 1;
    int neighbor, weight, newDist, dist, node;

    path_data.distance.assign(n, numeric_limits<int>::max());
    path_data.prev.assign(n, -1);    

    priority_queue<pii, vector<pii>, greater<pii>> pq;
    path_data.distance[path_data.start] = 0;
    pq.push({0, path_data.start});

    while (!pq.empty()) {

        auto top = pq.top();
        dist = top.first;
        node = top.second;

        pq.pop();

        if (dist > path_data.distance[node]) continue;

        if (node == path_data.end && break_early) break;

        for (int i = graph.start_indices[node]; i < graph.start_indices[node + 1]; ++i) {
            neighbor = graph.neighbors[i];
            weight = graph.weights[i];
            newDist = dist + weight;

            if (newDist < path_data.distance[neighbor]) {
                path_data.distance[neighbor] = newDist;
                path_data.prev[neighbor] = node;
                pq.push({newDist, neighbor});
            }
        }
    }
}

void dijkstra(Graph& graph, PathData& path_data, Timer& dijkstraTimer, bool break_early = false) {

    // Clear the path data
    path_data.path.clear();

    cout << "\nCalculating the shortest path ...\n" << endl;

    // Start the timer for the dijkstra algorithm
    start_timer(dijkstraTimer);

    // Run the dijkstra algorithm
    dijkstra_algorithm(graph, path_data, break_early);
    
    // Stop the timer for the dijkstra algorithm
    stop_timer(dijkstraTimer);

    // Build the path from the dijkstra results
    reconstructPath(path_data, dijkstraTimer);

    // Reset the path result
    path_data.prev.clear();

}


#endif