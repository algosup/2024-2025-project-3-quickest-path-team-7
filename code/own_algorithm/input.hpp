#ifndef INPUT_HPP
#define INPUT_HPP

#include "header.hpp"

void takeFolderInput(Files& files) {

    if (ASK_FOLDER){
        cout << "Do you want to change the folder for input/output files ? (y/n)" << flush;
        char c = 'x';
        while (c!='y' && c!='n') {
            cin >> c;
            if (c == 'y') {
                cout << "\nPlease provide the folder path: ";
                cin >> files.folder_path;
                files.dataset = files.folder_path + "/" + DATASET;
                files.output = files.folder_path + "/" + OUTPUT;
                files.map_backup = files.folder_path + "/" + MAP_BACKUP;
                files.landmarks_backup = files.folder_path + "/" + LANDMARKS_BACKUP;
            } else {
                if (c == 'n') {
                    files.folder_path = "";
                    files.dataset = DATASET;
                    files.output = OUTPUT;
                    files.map_backup = MAP_BACKUP;
                    files.landmarks_backup = LANDMARKS_BACKUP;
                } else {
                    cout << "Invalid input. Please try again." << endl;
                }
            }
        }
    } else {
        files.folder_path = "";
        files.dataset = DATASET;
        files.output = OUTPUT;
        files.map_backup = MAP_BACKUP;
        files.landmarks_backup = LANDMARKS_BACKUP;
    }
    
    cout << "\nThe program will read this dataset: " << files.dataset << endl;
    cout << "The program will save the output to this file: " << files.output << endl;
    cout << "The program will save a backup of the graph to these files: " << files.map_backup << " and " << files.landmarks_backup << "-X.bin" << endl;
}

int takeUserInput(Graph& graph, Path& path) {

    // Ask and check for the start and end nodes to calculate the shortest path
    cout << "\n\nEnter the start node: ";
    cin >> path.start;
   
    if (path.start == STOP) {
        return STOP; // Stop the program
    }

    if (path.start <= 0 || path.start > NODE_MAX_VALUE || path.start >= graph.map.size() || graph.map[path.start].empty()) 
    {
        cout << "Invalid node. Please try again." << endl;
        return INVALID_NODE; // Invalid node
    }
    cout << "Enter the end node: ";
    cin >> path.end;
    if (path.end <= 0 || path.end > NODE_MAX_VALUE || path.end >= graph.map.size() || graph.map[path.end].empty())
    {
        cout << "Invalid node. Please try again." << endl;
        return INVALID_NODE; // Invalid node
    }

    return 0;
}

#endif