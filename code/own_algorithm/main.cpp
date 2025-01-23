#include "header.hpp"

int main () {

    Graph graph;
    Files files;
    Path path_data;
    Timer timer;
    Astar astar_structs;

    takeFolderInput(files);

    loadGraph(graph, files);

    loadLandmarks(graph, files);

    preBuildAstarStructs(astar_structs, graph);

    //mapToConnexions(graph);

    while (true)
    {

        if (takeUserInput(graph, path_data) == INVALID_NODE)
        { 
            continue;

        } else if (path_data.start == STOP) {

            break;
        }

        start_timer(timer);

        find_path(graph, path_data, astar_structs);

        stop_timer(timer);
        path_data.calculation_time = timer.time;

        // If there is no path between the two nodes, output a message
        if (path_data.path.empty()) {
            cout << "No path found between node " << formatWithSpaces(path_data.start) << " and node " << formatWithSpaces(path_data.end) << "." << endl;
            continue;
        }

        // Output results
        cout << "\nTotal Distance     : "   << formatWithSpaces(path_data.distance)   << endl;
        cout <<   "Estimated Distance : " << formatWithSpaces(path_data.estimated_distance) << endl;
        cout <<   "Percentage Error   : "  << formatWithSpaces((path_data.distance - path_data.estimated_distance) * 100 / path_data.estimated_distance) << "%" << endl;
        cout <<   "Number of iterations: " << formatWithSpaces(astar_structs.iterations) << endl;
        cout <<   "Number of edges    : "   << formatWithSpaces(path_data.path.size() - 1) << endl;
        cout <<   "Calculation Time   : "   << formatWithSpaces(path_data.calculation_time)   << " " << TIME_UNIT_STR << endl;

        // Save the path nodes to a CSV file
        savePathToCSV(graph, files, path_data, astar_structs);        

    }

    cout << "End";
 
    cin.get(); // Pause the console

    return 0;

}