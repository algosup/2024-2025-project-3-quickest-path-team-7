#include "header.hpp"

#ifndef GRAPH_H
#define GRAPH_H

void preprocessGraph(const vector<vector<pii>>& graph, GraphData& graph_data) {
    cout << "Preprocessing the graph ..." << flush;
    int n = graph.size();

    graph_data.start_indices.resize(n + 1);
    for (int i = 0; i < n; ++i) {
        graph_data.start_indices[i] = graph_data.neighbors.size();
        for (const auto& p : graph[i]) {
            int neighbor = p.first;
            int weight = p.second;
            graph_data.neighbors.push_back(neighbor);
            graph_data.weights.push_back(weight);
        }
    }
    graph_data.start_indices[n] = graph_data.neighbors.size();
    cout << " Done !" << endl;
}

void saveGraphToBinary(const string& filename, const vector<vector<pii>>& graph) {
    ofstream file(filename, ios::binary);
    if (!file.is_open()) {
        cerr << "Failed to open the file for writing." << endl;
        return;
    }

    size_t size = graph.size();
    file.write(reinterpret_cast<const char*>(&size), sizeof(size));

    for (const auto& neighbors : graph) {
        size = neighbors.size();
        file.write(reinterpret_cast<const char*>(&size), sizeof(size));
        file.write(reinterpret_cast<const char*>(neighbors.data()), size * sizeof(pii));
    }

    file.close();
}

void loadGraphFromBinary(const string& filename, vector<vector<pii>>& graph) {
    ifstream file(filename, ios::binary);
    if (!file.is_open()) {
        cerr << "\nFailed to open the file for reading." << endl;
        return;
    }

    size_t size;
    file.read(reinterpret_cast<char*>(&size), sizeof(size));
    graph.resize(size);

    for (auto& neighbors : graph) {
        file.read(reinterpret_cast<char*>(&size), sizeof(size));
        neighbors.resize(size);
        file.read(reinterpret_cast<char*>(neighbors.data()), size * sizeof(pii));
    }

    file.close();
}

void loadGraph(vector<vector<pii>>& graph, bool force = false) {

    if (!force) {cout << "Loading the graph from the binary backup ..." << flush;}
    ifstream binaryFile(BACKUP, ios::binary);
    if (binaryFile.is_open() && !force) {
        binaryFile.close();
        loadGraphFromBinary(BACKUP, graph);
        cout << "Done !" << endl;
        // Calculate and display memory usage
        GraphMemoryUsage(graph);
        return;
    }

    if(!force){cout << "Any backup found" << endl;}

    // Load the CSV file in the graph
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
            graph[node1].push_back({node2, distance});
            graph[node2].push_back({node1, distance});
        }

        counter++;
        progression = counter * 100 / CSV_LINES;
        if (progression != progression_backup) {
            cout << "\rLoading the CSV file into memory ... " << progression << " %" << flush;
            progression_backup = progression;
        }
    }

    csv_map.close();

    // Save the graph to a binary file for future use
    saveGraphToBinary(BACKUP, graph);
    cout << "\nGraph saved to binary backup." << endl;

    // Calculate and display memory usage
    GraphMemoryUsage(graph);
}

#endif