#include "headers/header.hpp"

int main() {
    // To allow special characters in cout such as 'µ'
    setlocale(LC_ALL, "en_US.UTF-8");

    println("\n\n\n\nWelcome to the path finder !\nIf any problem occurs, type 'help' for a list of commands.", type::BOLD);

    // Build the i/o filenames and eventually ask for the folder path
    buildFilesPath(GlobalFiles);
    
    // try to load the graph and landmarks until it is successful
    while (!GlobalGraph.loaded) {
        loadGraph(GlobalGraph, GlobalFiles);
        if (!GlobalGraph.loaded) {
            println("The graph could not be loaded", type::ERROR);
            println("Press any key to retry loading the graph...", type::WARNING);
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
                println("Invalid node, must be between 1 and " + to_string(GlobalGraph.nodes_qty), type::WARNING);
                println("Please try again.", type::WARNING);
                continue;
                break;
            case INVALID_COMMAND :
                println("Invalid command, please try again.", type::WARNING);
                displayHelp();
                continue;
                break;
            case EXIT :
                println("Exiting... (user ask)", type::WARNING);
                return 0;
                break;
            case PATH :
                println("Find path from terminal...\n", type::INFO);
                print("Start node : ");
                println(GlobalPath.start, type::INFO);
                print("End node   : ");
                println(GlobalPath.end, type::INFO);
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