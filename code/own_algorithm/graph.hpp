#ifndef GRAPH_HPP
#define GRAPH_HPP

#include "header.hpp"

#ifndef GRAPH_H
#define GRAPH_H

#include "header.hpp"

void saveGraphToBinary(const string& filename, const vector<vector<int_pair>>& graph) {
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
        file.write(reinterpret_cast<const char*>(neighbors.data()), size * sizeof(int_pair));
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
    graph.map.resize(size);

    for (auto& neighbors : graph.map) {
        file.read(reinterpret_cast<char*>(&size), sizeof(size));
        neighbors.resize(size);
        file.read(reinterpret_cast<char*>(neighbors.data()), size * sizeof(int_pair));
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
            graph.map[node1].push_back({node2, distance});
            graph.map[node2].push_back({node1, distance});
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
    saveGraphToBinary(files.backup, graph.map);
    cout << "\nGraph saved to binary backup." << endl;
    
}

void buildGraph(Graph& graph, Files& files, bool force = false) {
    graph.map.resize(NODE_MAX_VALUE + 1);
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

}

#endif

#endif