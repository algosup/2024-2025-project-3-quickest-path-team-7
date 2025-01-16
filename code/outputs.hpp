#include "header.hpp"

#ifndef OUTPUTS_H
#define OUTPUTS_H

// Function to calculate then display the memory usage of the graph
void GraphMemoryUsage(const vector<vector<pii>>& graph) {
    size_t totalSize = 0;
    for (const auto& neighbors : graph) {
        totalSize += sizeof(neighbors) + (neighbors.size() * sizeof(pii));
    }

    // Adapt the division by 1024 acording to the size of the graph
    if (totalSize > 1024*1024) cout << "Memory used by the graph: " << totalSize/(1024*1024) << " MB" << endl;
    else if (totalSize > 1024) cout << "Memory used by the graph: " << totalSize/(1024) << " KB" << endl;
    else cout << "Memory used by the graph: " << totalSize << " Bytes" << endl;
}

// Function to format a number with spaces between each group of three digits
string formatWithSpaces(long number) {
    string numStr = to_string(number);
    int insertPosition = numStr.length() - 3;
    while (insertPosition > 0) {
        numStr.insert(insertPosition, " ");
        insertPosition -= 3;
    }
    return numStr;
}


void savePathToCSV(string filename, PathData& path_data) {
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "Failed to open the file for writing." << endl;
        return;
    }

    int last_node = path_data.path[path_data.path.size() - 1];

    file << "Nodes to go from " << formatWithSpaces(path_data.path[0]) << " to " << formatWithSpaces(last_node) 
    << "\nShortest path    : " << formatWithSpaces(path_data.distance[last_node]) 
    << "\nNumber of edges  : " << formatWithSpaces(path_data.path.size() - 1)
    << "\nCalculation time : " << formatWithSpaces(path_data.time) << " ms\n\n";

    int previous_distance = 0;
    for (int node : path_data.path) {
        file << node << ", " << path_data.distance[node] - previous_distance << "\n";
        previous_distance = path_data.distance[node];
    }
    file.close();

}

#endif