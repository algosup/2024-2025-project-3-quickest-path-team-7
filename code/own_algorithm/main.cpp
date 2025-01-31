#include "header.hpp"

int main () {

    // To allow special characters in cout such as 'µ'
    setlocale(LC_ALL, "en_US.UTF-8");

    Graph graph;
    Files files;
    Path path_data;
    Timer timer;
    Astar astar;

    cout << "\n\n\n\nWelcome to the path finder !\nIf any problem occurs, type 'help' for a list of commands." << endl;

    // Build the i/o filenames and eventually ask for the folder path
    takeFolderInput(files, SKIP);

    // try to load the graph
    while (!graph.loaded){
        switch(loadGraph(graph, files)) {
            case SUCCESS :
                graph.loaded = true;
                break;
            case NO_DATASET :
                takeFolderInput(files, ASK_FOLDER);
                break;
            case FAIL :
                cout << "Error loading graph, exiting... \n" << endl;
                return 1;
        }
    }

    while (true)
    {
        reset_compute_data(graph, path_data, astar); 

        // Take user input for the start and end nodes or any extra command (you can check it out)
        switch (takeUserInput(graph, path_data, files))
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
                cout << "Calculating shortest path from node " 
                     << formatWithSpaces(path_data.start) << " to node " 
                     << formatWithSpaces(path_data.end) << " ... " << endl;
                break;
            case COMMAND :
                continue;
                break;
            default:
                break;
        }

        find_path(graph, path_data, astar, timer);

        savePathToCSV(graph, files, path_data); 
    }

    cout << "Exiting... (something went wrong)" << endl;

    return 0;

}