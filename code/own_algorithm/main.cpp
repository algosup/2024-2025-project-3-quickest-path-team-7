#include "header.hpp"

int main () {

    Graph graph;
    Files files;
    Path path_data;
    Timer timer;
    Astar astar1, astar2;

    takeFolderInput(files);

    // Load the graph from CSV or backup
    if (loadGraph(graph, files) == FAIL) {
        cout << "Error loading graph, exiting... \n" << endl;
        return 1;
    }

    loadLandmarks(graph, files);

    reset_algorithm_data(graph, path_data, astar1, astar2);  
    
    path_data.start = 1;
    path_data.end = 2;
    find_path(graph, path_data, astar1, astar2);

    while (true)
    {
        // Take user input for the start and end nodes or any extra command (you can check it out)
        if (takeUserInput(graph, path_data) == INVALID_NODE){ 
            continue;
        } else if (path_data.start == STOP) {
            break;
        }

        cout << "Calculating shortest path between node " << formatWithSpaces(path_data.start) << " and node " << formatWithSpaces(path_data.end) << " ... " << endl;

        start_timer(timer);

        find_path(graph, path_data, astar1, astar2);

        stop_timer(timer);
        path_data.calculation_time = timer.time;

        // If there is no path between the two nodes, output a message
        if (path_data.path.empty()) {
            cout << "No path found between node " << formatWithSpaces(path_data.start) << " and node " << formatWithSpaces(path_data.end) << "." << endl;
            continue;
        }

        if (path_data.start == 1471291 && path_data.end == 9597648) {
            path_data.estimated_distance = 45789944;
        }
        if (path_data.start == 8 && path_data.end == 1200000) {
            path_data.estimated_distance = 30400830;
        }

        // Output results
        cout << "\nTotal Distance     : "   << formatWithSpaces(path_data.distance)   << endl;
        cout <<   "Estimated Distance : " << formatWithSpaces(path_data.estimated_distance) << endl;
        cout <<   "Percentage Error   : "  << formatWithSpaces((path_data.distance - path_data.estimated_distance) * 100 / path_data.estimated_distance) << "%" << endl;
        cout <<   "Number of edges    : "   << formatWithSpaces(path_data.path.size() - 1) << endl;
        cout <<   "Calculation Time   : "   << formatWithSpaces(path_data.calculation_time)   << " " << TIME_UNIT_STR << endl;

        // Save the path nodes to a CSV file
        savePathToCSV(graph, files, path_data, astar1, astar2);      
        reset_algorithm_data(graph, path_data, astar1, astar2);  

    }

    cout << "End";
 
    cin.get(); // Pause the console

    return 0;

}