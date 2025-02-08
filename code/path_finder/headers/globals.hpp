#ifndef GLOBALS_HPP
#define GLOBALS_HPP

#include "header.hpp"

// ---------- Global variables declarations ----------

// Constants
const int INF = numeric_limits<int>::max();

// Astars parameters
float   heuristic_weight    = WEIGHT;
int     landmarks_qty       = LANDMARKS_QTY;
int     root_landmark       = ROOT_LANDMARK;

// API synchronization
mutex graph_path_file_access;
atomic <bool> api_ready(false);
atomic <bool> kill_api(false);

// API parameters
int    port                     = PORT;
bool display_valid_requests     = DISP_VALID_REQ;
bool display_bad_requests       = DISP_BAD_REQ;
bool display_valid_responses    = DISP_VALID_RES;
bool display_error_responses    = DISP_ERR_RES;
string response_format          = "json";
string endpoint_adaptation      = ""; 

// Menu parameters
bool comparator_mode = false;


// ---------- Structures definitions ----------

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

struct file_path{
    string base;
    string full;
};

struct Files {
    string      working_directory;
    string      sub_folder  =   FOLDER;
    file_path   dataset     = { DATASET, "" };
    file_path   graph       = { GRAPH_BACKUP, "" };
    file_path   landmarks   = { LANDMARKS_BACKUP, "" };
    file_path   output      = { OUTPUT, "" };
    file_path   comp_output = { COMP_OUTPUT, "" };
    file_path   api_icon    = { API_ICON, "" };
};


// ---------- Global Strucutres declarations ----------

Graph GlobalGraph;
Files GlobalFiles;
Path  GlobalPath;
Timer GlobalTimer;
Astar GlobalAstar;

#endif