#ifndef GLOBALS_HPP
#define GLOBALS_HPP

#include "header.hpp"

float   heuristic_weight    = WEIGHT;
int     landmarks_qty       = LANDMARKS_QTY;
int     root_landmark       = ROOT_LANDMARK;

struct Timer {
    chrono::high_resolution_clock::time_point start_time;
    chrono::high_resolution_clock::time_point end_time;
    long time;
};

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

// Simple struct to store the neighbors of a node
struct Edge {
    int id;  
    int weight;  
};

// Astar data structures
struct Astar {
    vector<int> cost_from_start;
    vector<int_pair> node_before;
    vector<bool> checked;
    priority_queue<Node> pq;
    vector<Edge> neighbors;
};

// Graph in compressed adjacency form
struct Graph {    
    bool loaded = false;
    bool landmarks_loaded = false;
    // Number of nodes (max node ID)
    int nodes_qty;
    //   edges are stored in [ adjacency_start[u], adjacency_start[u+1] ) inside 'edges'
    // adjacency_start has length = map_size + 1 
    vector<int> adjacency_start;  
    // All edges for all nodes in a single contiguous array
    vector<Edge> edges;
    // list of landmarks IDs
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
    string root_landmarks_backup;
    string output;
};

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

Graph g_graph;
Files g_files;
Path  g_path;
Timer g_timer;
Astar g_astar;

#endif