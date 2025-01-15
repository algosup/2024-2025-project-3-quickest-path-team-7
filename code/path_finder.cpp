#include "header.hpp"

int main() {

    int start, end;                            // Example start and end
    vector<vector<pii>> graph(NODE_MAX_VALUE+1); // Graph is a vector of vectors of pairs of integers of size NODE_MAX_VALUE
    vector<int> distance, prev, path;          // Distance, previous node and path
    vector<int> start_indices, neighbors, weights;

    cout << "\nInitialization\n" << endl;

    // Load the graph from the CSV file or previous backup
    loadGraph(graph);

    // Preprocess the graph for Dijkstra
    preprocessGraph(graph, start_indices, neighbors, weights);

    // Main loop
    while (true) {

        cout << "\n\nEnter the start node: ";
        cin >> start;
        if (start == -1) {
            loadGraph(graph, true);
            continue; // Restart the loop
        }
        if (start < 0 || start >= graph.size() || graph[start].empty()) 
        {
            cout << "Invalid node. Please try again." << endl;
            continue;
        }
        cout << "Enter the end node: ";
        cin >> end;
        if (end < 0 || end >= graph.size() || graph[end].empty())
        {
            cout << "Invalid node. Please try again." << endl;
            continue;
        }

        cout << "\nCalculating the shortest path ...\n" << endl;
        // Measure the execution time
        auto start_time = chrono::high_resolution_clock::now();
        dijkstra_preloaded(start, end, distance, prev, start_indices, neighbors, weights);
        //dijkstra(start, end, graph, distance, prev);
        auto end_time = chrono::high_resolution_clock::now();

        //Calculate the time
        long time = chrono::duration_cast<chrono::milliseconds>(end_time - start_time).count();

        // Calculate the path
        path = reconstructPath(start, end, prev);

        if (path.empty()) {
            cout << "No path found between node " << formatWithSpaces(start) << " and node " << formatWithSpaces(end) << "." << endl;
        } else {
            // Save the path nodes to a CSV file
            savePathToCSV("shortest_path.csv", path, distance, time);

            // Output results
            cout << "\nTotal Distance   : " << formatWithSpaces(distance[end])   << endl;
            cout <<   "Number of edges  : " << formatWithSpaces(path.size() - 1) << endl;
            cout <<   "Calculation Time : " << formatWithSpaces(time)   << " ms" << endl;
        }
    }

    return 0;

}