#ifndef MENU_HPP
#define MENU_HPP

#include "header.hpp"

void displayHelp(){
    cout << "Commands: \n" << endl;
    cout << " - [integer]           : ask for a node Y to calculate the path between X and Y" << endl;
    cout << " - [string]            : read if it's a keyword for a preloaded path (such as fast, med, longest,...) and run it" << endl;
    cout << " - \"comparator\"      : activate the comparator mode to evaluate the difference between A* and dijkstra precision" << endl;
    cout << " - \"exit comparator\" : deactivate the comparator mode" << endl;
    cout << " - \"weight\"          : change the heuristic weight" << endl;
    cout << " - \"qty lm\"          : change the number of landmarks" << endl;
    cout << " - \"build lm\"        : rebuild the landmarks" << endl;
    cout << " - \"build graph\"     : rebuild the graph" << endl;
    cout << " - \"display lm\"      : display the landmarks" << endl;
    cout << " - \"pwd\"             : display the current working directory" << endl;
    cout << " - \"chfolder\"        : change the folder path for input/output files" << endl;
    cout << " - \"display files\"   : display the current files paths" << endl;
    cout << " - \"new dataset\"     : change the dataset .csv" << endl;
    cout << " - \"port\"            : change the port number" << endl;
    cout << " - \"display api\"     : display/hide the API responses and request specifically" << endl;
    cout << " - \"stop\"            : exit the program" << endl;
    cout << endl;
}

int takeUserInput(Graph& Graph, Path& path, Files& Files) {

    string input;
    if (comparator_mode) {
        cout << "\n\n!Comparator Mode!\nEnter a command or the start node : ";
    } else {
        cout << "\n\nEnter a command or the start node : ";
    }
    // cin. ignore if necessary :
    while (cin.peek() == '\n') {
        cin.ignore();
    }
    getline(cin, input);
   
    // Check for preloaded paths
    if (input == "fast") {
        path.start = 1;
        path.end = 2;
        return PATH;
    }
    if (input == "med") {
        path.start = 1;
        path.end = 23900000;
        return PATH;
    }
    if (input == "long1") {
        path.start = 9489093;
        path.end = 22377087;
        return PATH;
    }
    if (input == "long2") {
        path.start = 9489093;
        path.end = 22377087;
        return PATH;
    }
    if (input == "longest") {
        path.start = 9588784;
        path.end = 2720178;
        return PATH;
    }

    // Check for special commands
    if (input == "comparator") {
        comparator_mode = true;
        cout << "Comparator mode activated" << endl;
        return COMMAND;
    }
    if (input == "exit comparator") {
        comparator_mode = false;
        cout << "Comparator mode deactivated" << endl;
        return COMMAND;
    }
    if (input == "weight") {
        cout << "Enter the weight: ";
        cin >> heuristic_weight;
        cout << "Weight set to " << heuristic_weight << endl;
        return COMMAND;
    }
    if (input == "qty lm") {
        cout << "Enter the number of landmarks: ";
        cin >> landmarks_qty;
        loadLandmarks(Graph, Files);
        return COMMAND;
    }
    if (input == "build lm") {
        loadLandmarks(Graph, Files, FORCE_BUILD);
        return COMMAND;
    }
    if (input == "build graph") {
        loadGraph(Graph, Files, FORCE_BUILD);
        return COMMAND;
    }
    if (input == "display lm") {
        cout << "Landmarks: " << endl;
        for (int lm : Graph.landmarks) {
            cout << " - " << lm << " " << endl;
        }
        return COMMAND;
    }
    if (input == "pwd") {
        cout << "Working Directory: " << filesystem::current_path() << endl;
        return COMMAND;
    }
    if (input == "chfolder") {
        cout << "Please provide a relative path : ";
        cin >> Files.sub_folder;
        // if not ending with a slash, add it
        Files.sub_folder = Files.sub_folder.back() == '/' ? Files.sub_folder : Files.sub_folder + "/";
        buildFilesPath(Files);
        cout << "Files path changed to " << Files.sub_folder << endl;
        return COMMAND;
    }
    if (input == "display files") {
        cout << "Dataset          : " << Files.dataset.full << endl;
        cout << "graph backup     : " << Files.graph.full << endl;
        cout << "Landmarks backup : " << Files.landmarks.full << endl;
        cout << "Result output    : " << Files.output.full << endl;
        cout << "Compared output  : " << Files.comp_output.full << endl;
        cout << "API icon         : " << Files.api_icon.full << endl;

        return COMMAND;
    }
    if (input == "new dataset") {
        requireDataset(Files, NEW_ONE);
        cout << "Dataset set to " << Files.dataset.full << endl;
        loadGraph(Graph, Files);
        return COMMAND;
    }
    if (input == "port") {
        cout << "Enter the new port number: ";
        cin >> port;
        cout << "Port set to " << port << endl;
        kill_api.store(true);
        api_ready.store(false);
        thread(runApiServer).detach();
        return COMMAND;
    }
    if (input == "display api") {
        cout << "Which data do you want to display ?"       << endl;
        cout << " 1/-1 : display/hide the valid requests"   << endl;
        cout << " 2/-2 : display/hide the bad   requests"   << endl;
        cout << " 3/-3 : display/hide the valid responses"  << endl;
        cout << " 4/-4 : display/hide the error responses"  << endl;
        cout << " 5/-5 : display/hide all the above"        << endl;
        cout << "Option : ";

        int option;
        cin >> option;
        switch (option) {

        case 1:
            display_valid_requests = true;
            break;
        case -1:
            display_valid_requests = false;
            break;
        case 2:
            display_bad_requests = true;
            break;  
        case -2:
            display_bad_requests = false;
            break;
        case 3:
            display_valid_responses = true;
            break;
        case -3:
            display_valid_responses = false;
            break;
        case 4:
            display_error_responses = true;
            break;
        case -4:
            display_error_responses = false;
            break;
        case 5:
            display_valid_requests = true;
            display_bad_requests = true;
            display_valid_responses = true;
            display_error_responses = true;
            break;
        case -5:
            display_valid_requests = false;
            display_bad_requests = false;
            display_valid_responses = false;
            display_error_responses = false;
            break;
        default:
            cout << "Invalid option" << endl;
            break;
        }

        cout << "Display options updated" << endl;
        return COMMAND;
    }
    if (input == "help") {
        displayHelp();
        return COMMAND;
    }
    if (input == "exit") {
        return EXIT;
    }

    // Remove spaces between digits from input
    input.erase(remove(input.begin(), input.end(), ' '), input.end());
     // try to convert into an int
    try {
        path.start = stoi(input);
    } catch (const invalid_argument& e) {
        return INVALID_COMMAND; 
    } catch (const out_of_range& e) {
        return INVALID_NODE; 
    }

    if ( (path.start < 1 || path.start > Graph.nodes_qty) 
       || Graph.adjacency_start[path.start]==Graph.adjacency_start[path.start+1]){
        return INVALID_NODE; // Invalid node
    }

    input = "";
    cout << "Enter the end node                : ";
    while (cin.peek() == '\n') {
        cin.ignore();
    }
    getline(cin, input);
    // Remove spaces between digits from input
    input.erase(remove(input.begin(), input.end(), ' '), input.end());
     // try to convert into an int
    try {
        path.end = stoi(input);
    } catch (const invalid_argument& e) {
        return INVALID_NODE; 
    } catch (const out_of_range& e) {
        return INVALID_NODE; 
    }
    if ( (path.start < 1 || path.start > GlobalGraph.nodes_qty) 
       || Graph.adjacency_start[path.end]==Graph.adjacency_start[path.end+1]){
        return INVALID_NODE;
    }

    return PATH;
}

#endif