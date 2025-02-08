#include "headers/header.hpp"

int main() {
    // To allow special characters in cout such as 'µ'
    setlocale(LC_ALL, "en_US.UTF-8");

    cout << "\n\n\n\nWelcome to the path finder !\nIf any problem occurs, type 'help' for a list of commands." << endl;

    // Build the i/o filenames and eventually ask for the folder path
    buildFilesPath(GlobalFiles);
    
    // try to load the graph and landmarks until it is successful
    while (!GlobalGraph.loaded) {
        loadGraph(GlobalGraph, GlobalFiles);
        if (!GlobalGraph.loaded) {
            cout << "Press any key to retry loading the graph..." << endl;
            cin.get();
        }
    }
    
    api_ready.store(false);
    thread(runApiServer).detach();
    
    // Main loop for terminal access to the server
    while (true) {
        
        // wait for the API to be ready
        while (!api_ready.load()) {
            continue;
        }

        {
            lock_guard<mutex> lock(graph_path_file_access);
            // Reset the path data and the Astar data   
            resetComputeData(GlobalGraph, GlobalPath, GlobalAstar); 
        }

        // Take user input for the start and end nodes or any extra command (you can check it out)
        switch (takeUserInput(GlobalGraph, GlobalPath, GlobalFiles))
        {
            case INVALID_NODE :
                cout << "Invalid node, must be between 1 and " << GlobalGraph.nodes_qty << endl;
                cout << "Please try again." << endl;
                continue;
                break;
            case INVALID_COMMAND :
                cout << "Invalid command, please try again." << endl;
                displayHelp();
                continue;
                break;
            case EXIT :
                cout << "Exiting... (user ask)" << endl;
                return 0;
                break;
            case PATH :
                cout << "Find path from terminal...\n" << endl;
                break;
            case COMMAND :
                continue;
                break;
            default:
                break;
        }

        {
            lock_guard<mutex> lock(graph_path_file_access);
            // Calculate the shortest path, output results and reset the data
            
            findPath(GlobalGraph, GlobalPath, GlobalAstar, GlobalTimer);
            
            if (comparator_mode) {
                Path perfect_path;
                Astar precise_astar;
                Timer perfect_timer;
                resetComputeData(GlobalGraph, perfect_path, precise_astar); // serves as initialization too
                perfect_path.start = GlobalPath.start;
                perfect_path.end = GlobalPath.end;
                findPath(GlobalGraph, perfect_path, precise_astar, perfect_timer, NULL_HEURISTIC);
                displayComparisonResults(GlobalPath, perfect_path);
                saveComparedPathToCSV(GlobalFiles, GlobalPath, perfect_path);
                resetComputeData(GlobalGraph, GlobalPath, GlobalAstar);
            } else {
                displayResults(GlobalPath);
                savePathToCSV(GlobalFiles, GlobalPath);
                resetComputeData(GlobalGraph, GlobalPath, GlobalAstar);
            }
        }
    }

    return 0;
}