#ifndef OUTPUTS_H
#define OUTPUTS_H

#include "header.hpp"

struct Path {
    int start;
    int end;
    int distance;
    int calculation_time;
    int estimated_distance;
    // The path is stored as a vector of pairs {node, weight}
    vector<int_pair> path;
};


struct Node {
    int id;
    int weight;
    int estimated_cost;

    // Make the first Node in a priority queue the one with the smallest estimated distance
    bool operator<(const Node& other) const {
        return estimated_cost > other.estimated_cost;
    }
};

// Each edge in the compressed adjacency array
struct Edge {
    int id;  
    int weight;  
};


struct Astar {
    vector<int> cost_from_start;
    vector<int_pair> node_before;
    vector<bool> checked;
    priority_queue<Node> pq;
    vector<Edge> neighbors;
};

// Graph in compressed adjacency form
struct Graph {    
    // Number of nodes (max node ID)
    int nodes_qty;

    // For node u:
    //   edges are stored in [ adjacency_start[u], adjacency_start[u+1] ) inside 'edges'
    // adjacency_start has length = map_size + 1 
    vector<int> adjacency_start;  
    
    // All edges for all nodes in a single contiguous array
    vector<Edge> edges;

    // Additional data: landmarks etc.
    vector<int> landmarks;
    // distances from each landmark to each node
    // distance = landmark_distance[node * LANDMARK_QTY + landmark_index]
    vector<int> landmark_distance;
};

struct Files {
    string folder_path;
    string dataset;
    string map_backup;
    string landmarks_backup;
    string output;
};

// Global variables for shared data
atomic<bool> meeting_found(false);           // Meeting node found flag
atomic<int> meeting_node(0);                // Meeting node

mutex visited_mutex;

vector<bool> visited_forward;         // Forward visited set
vector<bool> visited_backward;        // Backward visited set

float heuristic_weight = WEIGHT;

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

void savePathToCSV(Graph& graph, Files& files, Path& path_data) {

    ofstream file(files.output);
    if (!file.is_open()) {
        cerr << "Failed to open the file for writing." << endl;
        return;
    }

    file 
    << "A-star Algorithm, "     << "using " << LANDMARKS_QTY << " landmarks"
    << "\nStart Node, "         << formatWithSpaces(path_data.start) 
    << "\nEnd Node, "           << formatWithSpaces(path_data.end) 
    << "\nEstimated Distance, " << formatWithSpaces(path_data.estimated_distance)
    << "\nPath lenght, "        << formatWithSpaces(path_data.distance) 
    << "\nNumber of nodes, "    << formatWithSpaces(path_data.path.size() - 1)
    << "\nCalculation time, "   << formatWithSpaces(path_data.calculation_time)      << " " << TIME_UNIT_STR
    << "\n\nNode,"              << " Weight\n";

    for (int_pair node : path_data.path) {
        file << node.first << ", " << node.second << "\n";
    }

    file.close();

}



#endif