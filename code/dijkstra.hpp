#include "header.hpp"

#ifndef DIJKSTRA_H
#define DIJKSTRA_H

/*

Basically, Dijkstra's algorithm is used to find the shortest path between two nodes in a graph.
It finds the shortest path from a starting node to all other nodes in the graph.
It scans ALL the possible paths before determining the shortest one.
So even if there is only one path between two nodes, it will scan all the paths to determine that it is the shortest.

It tooks the same time for any two nodes, no matter how far they are from each other.
Aproximately 10 seconds for a data set of 28,854,312 lines - 23,947,347 nodes. 
On a Mac boook pro M3 2024.

*/

void dijkstra_preloaded(int start, int end, vector<int>& distance, vector<int>& prev, const vector<int>& start_indices, const vector<int>& neighbors, const vector<int>& weights) {
    int n = start_indices.size() - 1;
    distance.assign(n, numeric_limits<int>::max());
    prev.assign(n, -1);

    priority_queue<pii, vector<pii>, greater<pii>> pq;
    distance[start] = 0;
    pq.push({0, start});

    while (!pq.empty()) {

        auto top = pq.top();
        int dist = top.first;
        int node = top.second;

        pq.pop();

        if (dist > distance[node]) continue;

        for (int i = start_indices[node]; i < start_indices[node + 1]; ++i) {
            int neighbor = neighbors[i];
            int weight = weights[i];
            int newDist = dist + weight;

            if (newDist < distance[neighbor]) {
                distance[neighbor] = newDist;
                prev[neighbor] = node;
                pq.push({newDist, neighbor});
            }
        }
    }
}

void dijkstra(int start, int end, const vector<vector<pii>>& graph, vector<int>& distance, vector<int>& prev) {

    int n = graph.size();
    distance.assign(n, numeric_limits<int>::max());
    prev.assign(n, -1);

    // Priority queue to store {distance, node}
    priority_queue<pii, vector<pii>, greater<pii>> pq;

    distance[start] = 0;
    pq.push({0, start});

    int recursion = 0;

    while (!pq.empty()) {

        /* recursion++;
        cout << "\r" << recursion << flush; */
        
        auto top = pq.top();
        int dist = top.first;
        int node = top.second;
        pq.pop();

        // Skip if this distance is not the latest
        if (dist > distance[node]) continue;

        for (const auto& edge : graph[node]) {
            int neighbor = edge.first;
            int weight = edge.second;
            int newDist = dist + weight;

            if (newDist < distance[neighbor]) {
                distance[neighbor] = newDist;
                prev[neighbor] = node;
                pq.push({newDist, neighbor});
            }
        }
    }
}

vector<int> reconstructPath(int start, int end, const vector<int>& prev) {
    vector<int> path;
    for (int at = end; at != -1; at = prev[at]) {
        path.push_back(at);
    }
    vector<int> reversedPath;
    for (auto it = path.rbegin(); it != path.rend(); ++it) {
        reversedPath.push_back(*it);
    }
    path = reversedPath;
    if (path[0] == start) return path;
    return {}; // No path found
}

#endif