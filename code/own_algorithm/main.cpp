#include "header.hpp"

int main () {

    Graph graph;
    Files files;
    Path path_data;
    Timer timer;

    takeFolderInput(files);

    loadGraph(graph, files);

    loadLandmarks(graph, files);

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

        find_path(graph, path_data);

        stop_timer(timer);
        path_data.calculation_time = timer.time;

        // If there is no path between the two nodes, output a message
        if (path_data.path.empty()) {
            cout << "No path found between node " << formatWithSpaces(path_data.start) << " and node " << formatWithSpaces(path_data.end) << "." << endl;
            continue;
        }

        // Output results
        cout << "\nTotal Distance   : " << formatWithSpaces(path_data.distance)   << endl;
        cout <<   "Number of edges  : " << formatWithSpaces(path_data.path.size() - 1) << endl;
        cout <<   "Calculation Time : " << formatWithSpaces(path_data.calculation_time)   << " ms" << endl;

        // Save the path nodes to a CSV file
        savePathToCSV(files, path_data);        

    }

    cout << "End";
 
    cin.get(); // Pause the console

    return 0;

}