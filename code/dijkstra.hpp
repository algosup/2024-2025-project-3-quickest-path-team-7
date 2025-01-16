#include "header.hpp"

#ifndef DIJKSTRA_H
#define DIJKSTRA_H

void dijkstra(GraphData& graph_data, PathData& path_data, bool break_early = false) {
    int n = graph_data.start_indices.size() - 1;
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

        for (int i = graph_data.start_indices[node]; i < graph_data.start_indices[node + 1]; ++i) {
            neighbor = graph_data.neighbors[i];
            weight = graph_data.weights[i];
            newDist = dist + weight;

            if (newDist < path_data.distance[neighbor]) {
                path_data.distance[neighbor] = newDist;
                path_data.prev[neighbor] = node;
                pq.push({newDist, neighbor});
            }
        }
    }
}

void reconstructPath(PathData& path_data, long time) {
    path_data.time = time;
    for (int at = path_data.end; at != -1; at = path_data.prev[at]) {
        path_data.path.push_back(at);
    }
    vector<int> reversedPath;
    for (auto it = path_data.path.rbegin(); it != path_data.path.rend(); ++it) {
        reversedPath.push_back(*it);
    }
    path_data.path = reversedPath;
    if (path_data.path[0] != path_data.start) {
        path_data.path.clear();
    }
}

#endif