#ifndef GRAPH_H
#define GRAPH_H

#include "header.hpp"

void preprocessGraph(Graph& graph) {

    cout << "Preprocessing the graph ..." << flush;
    int n = graph.data.size();

    graph.start_indices.resize(n + 1);
    for (int i = 0; i < n; ++i) {
        graph.start_indices[i] = graph.neighbors.size();
        for (const auto& p : graph.data[i]) {
            int neighbor = p.first;
            int weight = p.second;
            graph.neighbors.push_back(neighbor);
            graph.weights.push_back(weight);
        }
    }
    graph.start_indices[n] = graph.neighbors.size();
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

void loadGraphFromBinary(const string& filename, Graph& graph) {
    ifstream file(filename, ios::binary);
    if (!file.is_open()) {
        cerr << "\nFailed to open the file for reading." << endl;
        return;
    }

    size_t size;
    file.read(reinterpret_cast<char*>(&size), sizeof(size));
    graph.data.resize(size);

    for (auto& neighbors : graph.data) {
        file.read(reinterpret_cast<char*>(&size), sizeof(size));
        neighbors.resize(size);
        file.read(reinterpret_cast<char*>(neighbors.data()), size * sizeof(pii));
    }

    file.close();

    graph.loaded = true;
}

void loadGraph(Graph& graph, Files& files, bool force = false) {

    if (!force) {cout << "Loading the graph from the backup " << files.backup << " ..." << flush;}
    ifstream binaryFile(files.backup, ios::binary);
    if (binaryFile.is_open() && !force) {
        binaryFile.close();
        loadGraphFromBinary(files.backup, graph);
        cout << "Done !" << endl;
        return;
    }

    if(!force){cout << "\nThe backup " << files.backup << " was not found. \nSearching for " <<  files.dataset << " ..." << endl;}

    // Load the CSV file in the graph
    fstream csv_map(files.dataset);

    if (!csv_map.is_open()) {
        cerr << "The dataset " << files.dataset << " was not found. " << endl;
        graph.loaded = false;
        return;
    }

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
            graph.data[node1].push_back({node2, distance});
            graph.data[node2].push_back({node1, distance});
        }

        counter++;
        progression = counter * 100 / CSV_LINES;
        if (progression != progression_backup) {
            cout << "\rLoading the CSV file into memory ... " << progression << " %" << flush;
            progression_backup = progression;
        }
    }

    csv_map.close();

    graph.loaded = true;

    // Save the graph to a binary file for future use
    saveGraphToBinary(files.backup, graph.data);
    cout << "\nGraph saved to binary backup." << endl;

    
    
}

void buildGraph(Graph& graph, Files& files, bool force = false) {
    graph.data.resize(NODE_MAX_VALUE + 1);
    loadGraph(graph, files, force);   
    while (graph.loaded == false) {
        cout << "Please provide " << files.dataset << " , then type either 'retry' or a new dataset (\"path/file.csv\") : ";
        string input;
        cin >> input;
        if (input != "retry") {
            files.dataset = input;
        }
        cout << endl;
        loadGraph(graph, files, force);
    }
    preprocessGraph(graph);
    GraphMemoryUsage(graph);
}

#endif