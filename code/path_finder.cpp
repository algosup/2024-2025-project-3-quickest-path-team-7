#include "header.hpp"

int main() {

    bool break_early = false;                     // Break early flag
    char c;                                    // Character for break early
    vector<vector<pii>> graph(NODE_MAX_VALUE+1); // Graph is a vector of vectors of pairs of integers of size NODE_MAX_VALUE
    GraphData graph_data;                            // Data structure
    PathData  path_data;                             // Data structure

    cout << "\nInitialization\n" << endl;

    // Load the graph from the CSV file or previous backup
    loadGraph(graph);

    // Preprocess the graph for Dijkstra
    preprocessGraph(graph, graph_data);

    // Main loop
    while (true) {

        cout << "\n\nEnter the start node: ";
        cin >> path_data.start;
        if (path_data.start == -1) {
            loadGraph(graph, true);
            continue; // Restart the loop
        }
        if (path_data.start < 0 || path_data.start >= graph.size() || graph[path_data.start].empty()) 
        {
            cout << "Invalid node. Please try again." << endl;
            continue;
        }
        cout << "Enter the end node: ";
        cin >> path_data.end;
        if (path_data.end < 0 || path_data.end >= graph.size() || graph[path_data.end].empty())
        {
            cout << "Invalid node. Please try again." << endl;
            continue;
        }

        // Ask the user if they want to break early the dijkstra algorithm
        c = 'x';
        while (c!='y' && c!='n') {
            cout << "Break early (y/n)? ";
            cin >> c;
            if (c == 'y') {
                break_early = true;
            } else {
                if (c == 'n') {
                    break_early = false;
                } else {
                    cout << "Invalid input. Please try again." << endl;
                }
            }
        } 

        cout << "\nCalculating the shortest path ...\n" << endl;
        // Measure the execution time
        auto start_time = chrono::high_resolution_clock::now();

        dijkstra(graph_data, path_data, break_early);

        auto end_time = chrono::high_resolution_clock::now();

        //Calculate the time
        long time = chrono::duration_cast<chrono::milliseconds>(end_time - start_time).count();

        // Calculate the path
        reconstructPath(path_data, time);

        if (path_data.path.empty()) {
            cout << "No path found between node " << formatWithSpaces(path_data.start) << " and node " << formatWithSpaces(path_data.end) << "." << endl;
        } else {
            // Save the path nodes to a CSV file
            savePathToCSV(OUTPUT, path_data);

            // Output results
            cout << "\nTotal Distance   : " << formatWithSpaces(path_data.distance[path_data.end])   << endl;
            cout <<   "Number of edges  : " << formatWithSpaces(path_data.path.size() - 1) << endl;
            cout <<   "Calculation Time : " << formatWithSpaces(time)   << " ms" << endl;
        }
    }

    return 0;

}