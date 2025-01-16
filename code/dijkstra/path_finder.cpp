#include "header.hpp"

int main() {

    bool break_early = false;
    Graph graph;
    PathData  path_data;
    Timer dijkstraTimer;

    cout << "\nInitialization\n" << endl;

    // Load Csv data in memory as graph eventually from a backup
    buildGraph(graph);
    
    // Main loop
    while (true) {
        
        // Ask for the start and end nodes as well as the break_early option to stop the dijkstra algorithm when the end node is reached
        if (takeUserInput(path_data, graph, break_early) == -1) { continue;}

        // Run the dijkstra algorithm
        dijkstra(graph, path_data, dijkstraTimer, break_early);

        // If there is no path between the two nodes, output a message
        if (path_data.path.empty()) {
            cout << "No path found between node " << formatWithSpaces(path_data.start) << " and node " << formatWithSpaces(path_data.end) << "." << endl;
            continue;
        }

        // Save the path nodes to a CSV file
        savePathToCSV(OUTPUT, path_data);

        // Output results
        cout << "\nTotal Distance   : " << formatWithSpaces(path_data.distance[path_data.end])   << endl;
        cout <<   "Number of edges  : " << formatWithSpaces(path_data.path.size() - 1) << endl;
        cout <<   "Calculation Time : " << formatWithSpaces(dijkstraTimer.time)   << " ms" << endl;
    }

    return 0;

}