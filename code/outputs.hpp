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


void savePathToCSV(const string& filename, const vector<int>& path, const vector<int>& distance, const long& time) {
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "Failed to open the file for writing." << endl;
        return;
    }

    file << "Nodes to go from " << formatWithSpaces(path[0]) << " to " << formatWithSpaces(path[path.size() - 1]) 
    << "\nShortest path    : " << formatWithSpaces(distance[path[path.size() - 1]]) 
    << "\nNumber of edges  : " << formatWithSpaces(path.size() - 1)
    << "\nCalculation time : " << formatWithSpaces(time) << " ms\n\n";

    int previous_distance = 0;
    for (int node : path) {
        file << node << ", " << distance[node] - previous_distance << "\n";
        previous_distance = distance[node];
    }

    file.close();
}

#endif