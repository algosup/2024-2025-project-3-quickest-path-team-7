#ifndef OUTPUTS_H
#define OUTPUTS_H

#include "header.hpp"

struct Path {

    int start;
    int end;
    int distance;
    int calculation_time;
    // The path is stored as a vector of pairs {node, weight}
    vector<int_pair> path;
};

struct Graph {

    bool loaded = false;

    // map[node] stores the list of neighbors of the node and their weights as {neighbor, weight}
    vector<vector<int_pair>> map;

    // connexions[node] stores the list of neighbors of the node
    vector<vector<int>> connexions;

    // the list of the landmarks picked
    vector<int> landmarks;

    // The landmark distance table
    vector<vector<int>> landmark_distance;
    // Considering landmark_index is the index of the landmark in the landmarks vector :
    // To build this table the distance is stored as : landmark_distance[landmark_index][node] = distance 
    // For a faster build as the preprocessing iterates on the nodes for a given landmark
    
    // At the end of the prepreocessing, we reverse the access : 
    // the distances are stored as: landmark_distance[node][landmark_index] = distance
    // For a faster access as A* iterates on the landmarks for a given node

};

struct Files {
    string folder_path;
    string dataset;
    string map_backup;
    string landmarks_backup;
    string output;
};

// Function to calculate then display the memory usage of the graph
void GraphMemoryUsage(Graph& graph) {

    size_t totalSize = 0;
    // map size
    for (const auto& neighbors : graph.map) {
        totalSize += sizeof(neighbors) + (neighbors.size() * sizeof(int_pair));
    }
    // landmarks size 
    totalSize += graph.landmarks.size() * sizeof(int);

    // landmark_distance size
    for (const auto& distances : graph.landmark_distance) {
        totalSize += sizeof(distances) + (distances.size() * sizeof(int));
    }

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

void savePathToCSV(Files& files, Path& path_data) {

    ofstream file(files.output);
    if (!file.is_open()) {
        cerr << "Failed to open the file for writing." << endl;
        return;
    }

    file 
    << "A-star Algorithm, "     << "using " << LANDMARKS_QTY << " landmarks"
    << "\nStart Node, "         << formatWithSpaces(path_data.start) 
    << "\nEnd Node, "           << formatWithSpaces(path_data.end) 
    << "\nPath lenght, "        << formatWithSpaces(path_data.distance) 
    << "\nNumber of nodes, "    << formatWithSpaces(path_data.path.size() - 1)
    << "\nCalculation time, "   << formatWithSpaces(path_data.calculation_time)      << " ms"
    << "\n\nNode,"              << " Weight\n";

    for (int_pair node : path_data.path) {
        file << node.first << ", " << node.second << "\n";
    }

    file.close();

    // reset the path_data
    path_data.path.clear();
    path_data.distance = 0;

}


#endif