#ifndef INPUT_HPP
#define INPUT_HPP

#include "header.hpp"

void takeFolderInput(Files& files, bool ask_folder = SKIP) {

    files.folder_path = "";
    files.dataset = DATASET;
    files.output = OUTPUT;
    files.map_backup = MAP_BACKUP;
    files.root_landmarks_backup = LANDMARKS_BACKUP;

    if (ask_folder){
        char cwd[PATH_MAX];
        if (getcwd(cwd, sizeof(cwd)) != NULL) {
            cout << "Working Directory: " << cwd << endl;
        }
        cout << "Do you want to use a specific relative path ? (y/n) ";
        string answer;
        cin >> answer;
        if (answer == "y") {
            cout << "Please provide a relative path : ";
            cin >> files.folder_path;
            files.dataset = files.folder_path + "/" + DATASET;
            files.output = files.folder_path + "/" + OUTPUT;
            files.map_backup = files.folder_path + "/" + MAP_BACKUP;
            files.root_landmarks_backup = files.folder_path + "/" + LANDMARKS_BACKUP;
        } else {
            cout << "Trying again in the current folder ... " << endl;
        }
    }

    files.landmarks_backup = files.root_landmarks_backup + "-" + to_string(landmarks_qty) + ".bin";
}

int takeUserInput(Graph& graph, Path& path, Files& files) {

    string input;
    // Ask and check for the start and end nodes to calculate the shortest path
    cout << "\n\nEnter a command or the start node : ";
    cin >> input;
   
    // Check for preloaded paths
    if (input == "fast") {
        path.start = 1;
        path.end = 2;
        return PATH;
    }
    if (input == "med") {
        path.start = 1471291;
        path.end = 9597648;
        return PATH;
    }
    if (input == "long") {
        path.start = 9489093;
        path.end = 22377087;
        return PATH;
    }
    if (input == "try") {
        path.start = 1471291;
        path.end = 22377087;
        return PATH;
    }

    // Check for special commands
    if (input == "weight") {
        cout << "Enter the weight: ";
        cin >> heuristic_weight;
        cout << "Weight set to " << heuristic_weight << endl;
        return COMMAND;
    }
    if (input == "qty-lm") {
        cout << "Enter the number of landmarks: ";
        cin >> landmarks_qty;
        loadLandmarks(graph, files);
        return COMMAND;
    }
    if (input == "build-lm") {
        loadLandmarks(graph, files, FORCE_BUILD);
        return COMMAND;
    }
    if (input == "build-graph") {
        loadGraph(graph, files, FORCE_BUILD);
        loadLandmarks(graph, files, FORCE_BUILD);
        return COMMAND;
    }
    if (input == "display-lm") {
        cout << "Landmarks: " << endl;
        for (int lm : graph.landmarks) {
            cout << " - " << lm << " " << endl;
        }
        return COMMAND;
    }
    if (input == "chfolder") {
        cout << "Enter the new folder path: ";
        cin >> files.folder_path;
        files.dataset = files.folder_path + "/" + DATASET;
        files.output = files.folder_path + "/" + OUTPUT;
        files.map_backup = files.folder_path + "/" + MAP_BACKUP;
        files.root_landmarks_backup = files.folder_path + "/" + LANDMARKS_BACKUP;
        files.landmarks_backup = files.root_landmarks_backup + "-" + to_string(landmarks_qty) + ".bin";
        cout << "Files path changed to " << files.folder_path << endl;
        return COMMAND;
    }
    if (input == "display-files") {
        cout << "Dataset: " << files.dataset << endl;
        cout << "Result output: " << files.output << endl;
        cout << "graph backup: " << files.map_backup << endl;
        cout << "Landmarks backup: " << files.landmarks_backup << endl;
        return COMMAND;
    }
    if (input == "exit") {
        return EXIT;
    }
    if (input == "help") {
        cout << "Commands: \n" << endl;
        cout << " - (integer)       : ask for a node Y to calculate the path between X and Y" << endl;
        cout << " - fast            : calculate the path between nodes 1 and 2" << endl;
        cout << " - med             : calculate the path between nodes 1471291 and 9597648" << endl;
        cout << " - long            : calculate the path between nodes 9489093 and 22377087" << endl;
        cout << " - try             : calculate the path between nodes 1471291 and 22377087" << endl;
        cout << " - weight          : change the heuristic weight" << endl;
        cout << " - qty-lm          : change the number of landmarks" << endl;
        cout << " - build-lm        : rebuild the landmarks" << endl;
        cout << " - build-graph     : rebuild the graph" << endl;
        cout << " - display-lm      : display the landmarks" << endl;
        cout << " - chfolder        : change the folder path for input/output files" << endl;
        cout << " - display-files   : display the current files paths" << endl;
        cout << " - stop            : exit the program" << endl;
        cout << endl;
        return COMMAND;
    }

    // try to convert into an int, if it can't, it returns invalid node
    try {
        path.start = stoi(input);
    } catch (const invalid_argument& e) {
        return INVALID_COMMAND; // Invalid node
    } catch (const out_of_range& e) {
        return INVALID_COMMAND; // Invalid node
    }

    if ( !(1 <= path.start <= graph.nodes_qty) || graph.adjacency_start[path.start]==graph.adjacency_start[path.start+1]){
        return INVALID_NODE; // Invalid node
    }

    cout << "Enter the end node: ";
    cin >> path.end;
    if ( !(1 <= path.end <= graph.nodes_qty) || graph.adjacency_start[path.end]==graph.adjacency_start[path.end+1]){
        return INVALID_NODE; // Invalid node
    }

    return PATH;
}

#endif