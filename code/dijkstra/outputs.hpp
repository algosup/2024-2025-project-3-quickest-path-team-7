#ifndef OUTPUTS_H
#define OUTPUTS_H

#include "header.hpp"

struct PathData {
    int start;
    int end;
    long time;
    vector<int> distance;
    vector<int> prev;
    vector<int> path;
};

struct Graph {
    vector<vector<pii>> data;
    vector<int> start_indices;
    vector<int> neighbors;
    vector<int> weights;
};

// Function to calculate then display the memory usage of the graph
void GraphMemoryUsage(Graph& graph) {

    size_t totalSize = 0;
    for (const auto& neighbors : graph.data) {
        totalSize += sizeof(neighbors) + (neighbors.size() * sizeof(pii));
    }
    // Then graph.start_indices
    totalSize += sizeof(graph.start_indices) + (graph.start_indices.size() * sizeof(int));
    // Then graph.neighbors
    totalSize += sizeof(graph.neighbors) + (graph.neighbors.size() * sizeof(int));
    // Finally graph.weights
    totalSize += sizeof(graph.weights) + (graph.weights.size() * sizeof(int));


    // Adapt the division by 1024 acording to the size of the graph (from Byte to GB) in 2 digit precision
    if (totalSize > 1024 * 1024 * 1024) {
        cout << "Memory used by the Graph: " << fixed << setprecision(2) << (double)totalSize / (1024 * 1024 * 1024) << " GB" << endl;
    } else if (totalSize > 1024 * 1024) {
        cout << "Memory used by the Graph: " << fixed << setprecision(2) << (double)totalSize / (1024 * 1024) << " MB" << endl;
    } else if (totalSize > 1024) {
        cout << "Memory used by the Graph: " << fixed << setprecision(2) << (double)totalSize / 1024 << " KB" << endl;
    } else {
        cout << "Memory used by the Graph: " << fixed << setprecision(2) << (double)totalSize << " Bytes" << endl;
    }
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

void reconstructPath(PathData& path_data, Timer& timer) {
    int node = path_data.end;
    while (node != -1) {
        path_data.path.push_back(node);
        node = path_data.prev[node];
    }
    vector<int> reversedPath;
    for (auto it = path_data.path.rbegin(); it != path_data.path.rend(); ++it) {
        reversedPath.push_back(*it);
    }
    path_data.path = reversedPath;
    path_data.time = timer.time;
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