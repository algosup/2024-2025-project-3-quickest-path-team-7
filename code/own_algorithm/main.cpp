#include "headers/header.hpp"

int main() {
    // To allow special characters in cout such as 'µ'
    setlocale(LC_ALL, "en_US.UTF-8");

    cout << "\n\n\n\nWelcome to the path finder !\nIf any problem occurs, type 'help' for a list of commands." << endl;

    // Build the i/o filenames and eventually ask for the folder path
    takeFolderInput(g_files, SKIP);

    // try to load the graph and landmarks until it is successful
    while (!g_graph.loaded) {
        switch(loadGraph(g_graph, g_files)) {
            case SUCCESS:
                g_graph.loaded = true;
                break;
            case NO_DATASET:
                takeFolderInput(g_files, ASK_FOLDER);
                break;
            case FAIL:
                cerr << "Error loading graph, exiting..." << endl;
                return 1;
        }
    }
    
    api_ready.store(false);
    thread(run_api_server).detach();
    
    // Main loop for terminal access to the server
    while (true) {
        
        // wait for the API to be ready
        while (!api_ready.load()) {
            continue;
        }

        {
            lock_guard<mutex> lock(graph_path_file_access);
            // Reset the path data and the Astar data   
            reset_compute_data(g_graph, g_path, g_astar); 
        }

        // Take user input for the start and end nodes or any extra command (you can check it out)
        switch (takeUserInput(g_graph, g_path, g_files))
        {
            case INVALID_NODE :
                cout << "Invalid node, please try again." << endl;
                continue;
                break;
            case INVALID_COMMAND :
                cout << "Invalid command, please try again." << endl;
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
            
            find_path(g_graph, g_path, g_astar, g_timer);
            displayResults(g_path);
            savePathToCSV(g_files, g_path);
            reset_compute_data(g_graph, g_path, g_astar);
        }
    }

    return 0;
}