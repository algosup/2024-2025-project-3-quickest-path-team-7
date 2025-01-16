#ifndef DAG_HPP
#define DAG_HPP

#include "header.hpp"

// Function to detect a cycle in a directed graph using DFS
bool dfs(int node, vector<vector<int>>& graph, vector<int>& visited) {
    // 0 = not visited, 1 = visiting (in the current recursion stack), 2 = visited
    visited[node] = 1;  // Mark the node as visiting

    // Explore all neighbors
    for (int neighbor : graph[node]) {
        if (visited[neighbor] == 1) {  // A cycle is detected
            return true;
        }
        if (visited[neighbor] == 0 && dfs(neighbor, graph, visited)) {
            return true;
        }
    }

    visited[node] = 2;  // Mark the node as fully processed
    return false;
}

void buildDag() {

    vector<vector<int>> dag;

    loadDAG(dag);

    // To track the state of the nodes during DFS (0 = unvisited, 1 = visiting, 2 = visited)
    int n = dag.size();
    vector<int> visited(n, 0);  // 0 = unvisited, 1 = visiting, 2 = visited

    // Check for cycles in the graph
    for (int i = 0; i < n; ++i) {
        if (visited[i] == 0) {  // If the node is not visited
            if (dfs(i, dag, visited)) {
                cout << "The graph contains a cycle!" << endl;
                return 0;
            }
        }
    }

    cout << "The graph is acyclic (DAG)." << endl;
}

#endif