#include "header.hpp"

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

    thread(run_api_server).detach();
    
    // Maintenir le programme actif
    while (true) {
        string command;
        cout << "\nEnter 'exit' to quit: ";
        getline(cin, command);
        if (command == "exit") break;
    }

    return 0;
}