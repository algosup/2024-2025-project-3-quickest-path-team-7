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

void loadDAG(vector<vector<int>>& dag, bool force = false) 
{

    fstream csv_map(DATASET);

    unsigned int counter = 0;
    unsigned int progression = 0;
    unsigned int progression_backup = 0;

    string line;

    while (getline(csv_map, line)) {


        stringstream nodeString(line);
        string node_cell;
        vector<int> nodeData;
        while (getline(nodeString, node_cell, ',')) {
            nodeData.push_back(stoi(node_cell));
        }

        // CSV format is: node1, node2, distance
        if (nodeData.size() == 3) {
            int node1 = nodeData[0];
            int node2 = nodeData[1];
            int distance = nodeData[2];
            dag[node1] = {node2, distance};
        }

        counter++;
        progression = counter * 100 / CSV_LINES;
        if (progression != progression_backup) {
            cout << "\rLoading the CSV file into memory ... " << progression << " %" << flush;
            progression_backup = progression;
        }
    }

    csv_map.close();

}

void buildDag(vector<vector<int>>& dag) {

    loadDAG(dag);

    // To track the state of the nodes during DFS (0 = unvisited, 1 = visiting, 2 = visited)
    int n = dag.size();
    vector<int> visited(n, 0);  // 0 = unvisited, 1 = visiting, 2 = visited

    // Check for cycles in the graph
    for (int i = 0; i < n; ++i) {
        if (visited[i] == 0) {  // If the node is not visited
            if (dfs(i, dag, visited)) {
                cout << "The graph contains a cycle!" << endl;
                return;
            }
        }
    }

    cout << "The graph is acyclic (DAG)." << endl;
}

#endif