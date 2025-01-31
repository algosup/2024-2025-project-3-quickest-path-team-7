#ifndef SCANNER_H   
#define SCANNER_H

#include "header.hpp"

struct path_estim {
    int estimation;
    int start;
    int end;

    // Comparison operator for the priority queue (min-heap)
    bool operator<(const path_estim& other) const {
        return estimation > other.estimation;
    }
};

// Function which uses 'estimate_distance' to find the 1% longest paths
// Then use find_path to calculate the shortest path between the two nodes
// Order them by the time of execution in a csv file with for each line :
// start node, end node, estimation, path distance, time of calculation

void scanLongestPaths(Graph& graph, Files& files, Path& path_data, Astar& astar) {

    unsigned int counter = 0;
    unsigned int progression = 0;
    unsigned int progression_backup = 0;

    Timer timer;
    priority_queue<path_estim> longest_paths;

    // Open the file to write the results
    ofstream file(files.scan_result);
    if (!file.is_open()) {
        cerr << "Failed to open the file for writing." << endl;
        return;
    }

    // Write the header of the CSV file
    file << "process time, edges quantity, total weight, start node, end node\n";

    // For each node
    for (int node = 1; node <= graph.nodes_qty; ++node) {
        // For each other node
        for (int other_node = 1; other_node <= graph.nodes_qty; ++other_node) {
            // Skip the same node
            if (node == other_node) continue;

            // Estimate the distance between the two nodes and push it to the priority queue
            longest_paths.push({estimate_distance(graph, node, other_node), node, other_node});
            
            counter++;
            progression = counter * 100 / CSV_LINES;
            if (progression != progression_backup) {
                cout << "\rScanning all the path by estimation ... " << progression << " %" << flush;
                progression_backup = progression;
            }
        }
    }

    cout << "\rScanning all the path by estimation ... 100 %" << endl;

    counter = 0;
    progression = 0;
    progression_backup = 0;

    // For each path in the priority queue, calculate the shortest path
    while (!longest_paths.empty()) {

        // Take out the path with the longest estimated distance
        path_estim the_path = longest_paths.top();
        path_data.start = the_path.start;
        path_data.end = the_path.end;
        longest_paths.pop();
        
        // Estimate the distance between the two nodes 
        start_timer(timer);
        astar_algorithm(graph, path_data, astar);
        stop_timer(timer);
        path_data.calculation_time = timer.time;

        // Write the results to the file
        // shape : process time, edges quantity, total weight, start node, end node
        file
        << formatWithSpaces(path_data.calculation_time) << ", "
        << formatWithSpaces(path_data.path.size() - 1) << ", "
        << formatWithSpaces(path_data.distance) << ", "
        << formatWithSpaces(path_data.start) << ", "
        << formatWithSpaces(path_data.end) << "\n";

        counter++;
        progression = counter * 100 / CSV_LINES;
        if (progression != progression_backup) {
            cout << "\rComputing all the shortest paths (higher estimations first) ... " << progression << " %" << flush;
            progression_backup = progression;
        }
    }

    cout << "\rComputing all the shortest paths (higher estimations first) ... 100 %" << endl;

    file.close();
}

#endif