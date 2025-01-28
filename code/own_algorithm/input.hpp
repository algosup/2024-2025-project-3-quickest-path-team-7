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

    files.landmarks_backup = files.landmarks_backup + "-" + to_string(LANDMARKS_QTY) + ".bin";
    
    cout << "\nThe program will read this dataset: " << files.dataset << endl;
    cout << "The program will save the output to this file: " << files.output << endl;
    cout << "The program will save a backup of the graph to these files: " << files.map_backup << " and " << files.landmarks_backup << endl;
}

int takeUserInput(Graph& graph, Path& path) {

    string input;
    // Ask and check for the start and end nodes to calculate the shortest path
    cout << "\n\nEnter the start node: ";
    cin >> input;
   
    if (input == "exit") {
        return STOP; // Stop the program
    }

    if (input == "fast") {
        cout << "Using predefined nodes 1 and 2." << endl;
        path.start = 1;
        path.end = 2;
        return 0;
    }

    if (input == "med") {
        cout << "Using predefined nodes 8 and 1200000." << endl;
        path.start = 8;
        path.end = 1200000;
        return 0;
    }

    if (input == "long") {
        cout << "Using predefined nodes 1471291 and 9597648." << endl;
        path.start = 1471291;
        path.end = 9597648;
        return 0;
    }

    if (input == "weight") {
        cout << "Enter the weight: ";
        cin >> heuristic_weight;
        cout << "Weight set to " << heuristic_weight << endl;
        return 0;
    }

    // try to convert into an int, if it can't, it returns invalid node
    try {
        path.start = stoi(input);
    } catch (const invalid_argument& e) {
        cout << "Invalid expression. Please try again." << endl;
        return INVALID_NODE; // Invalid node
    } catch (const out_of_range& e) {
        cout << "Invalid expression. Please try again." << endl;
        return INVALID_NODE; // Invalid node
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