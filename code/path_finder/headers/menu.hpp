#ifndef MENU_HPP
#define MENU_HPP

#include "header.hpp"

void displayHelp(){
    println("HELP - Commands: \n", type::BOLD);
    println(" - [integer]         : ask for a node Y to calculate the path between X and Y");
    println(" - [string]          : read if it's a keyword for a preloaded path (such as fast, med, longest,...) and run it");
    println(" - \"comparator\"      : activate the comparator mode to evaluate the difference between A* and dijkstra precision");
    println(" - \"exit comparator\" : deactivate the comparator mode");
    println(" - \"weight\"          : change the heuristic weight");
    println(" - \"qty lm\"          : change the number of landmarks");
    println(" - \"build lm\"        : rebuild the landmarks");
    println(" - \"build graph\"     : rebuild the graph");
    println(" - \"display lm\"      : display the landmarks");
    println(" - \"pwd\"             : display the current working directory");
    println(" - \"new dataset\"     : change the dataset .csv");
    println(" - \"files location\"  : change the location of input/output files");
    println(" - \"display files\"   : display the current files paths");
    println(" - \"new port\"        : change the port number");
    println(" - \"display api\"     : display/hide the API responses and request specifically");
    println(" - \"stop\"            : exit the program");
    println("");
}

int takeUserInput(Graph& Graph, Path& path, Files& Files) {

    string input;
    if (comparator_mode) {
        print("\n\n->Comparator Mode ↴ \nEnter a command or the start node : ", type::INFO);
    } else {
        print("\n\nEnter a command or the start node : ");
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
        path.start = 15332231;
        path.end = 14538829;
        return PATH;
    }

    // Check for special commands
    if (input == "comparator") {
        comparator_mode = true;
        println("Comparator mode activated");
        return COMMAND;
    }
    if (input == "exit comparator") {
        comparator_mode = false;
        println("Comparator mode deactivated");
        return COMMAND;
    }
    if (input == "weight") {
        print("Enter the weight: ");
        cin >> heuristic_weight;
        println("Weight set to " + to_string(heuristic_weight));
        return COMMAND;
    }
    if (input == "qty lm") {
        print("Enter the number of landmarks: ");
        cin >> landmarks_qty;
        loadLandmarks(Graph, Files);
        return COMMAND;
    }
    if (input == "build lm") {
        loadLandmarks(Graph, Files, FORCE_BUILD);
        return COMMAND;
    }
    if (input == "build graph") {
        // try to load the graph and landmarks until it is successful
        loadGraph(GlobalGraph, GlobalFiles, FORCE_BUILD);
        if (!GlobalGraph.loaded) {
            println("The graph could not be loaded. Reloading the previous dataset from backup...", type::ERROR);
            loadGraph(GlobalGraph, GlobalFiles);
        }
        if (!GlobalGraph.loaded) {
            println("The graph could not be loaded. Exiting...", type::ERROR);
            return EXIT;
        }
        return COMMAND;
    }
    if (input == "display lm") {
        println("Landmarks: ");
        for (int lm : Graph.landmarks) {
            println(" - " + to_string(lm));
        }
        return COMMAND;
    }
    if (input == "pwd") {
        println("Working Directory: " + filesystem::current_path().string());
        return COMMAND;
    }
    if (input == "new dataset") {
        requireDataset(Files, NEW_ONE);
        println("Dataset set to " + Files.dataset.full, type::VALIDATION);
        loadGraph(Graph, Files);
        return COMMAND;
    }
    if (input == "files location") {
        newLocation(Files);
        requireDataset(Files, ASK_FOLDER);
        println("Files location updated", type::VALIDATION);
        loadGraph(Graph, Files);
        return COMMAND;
    }
    if (input == "display files") {
        println("Dataset          : " + Files.dataset.full);
        println("graph backup     : " + Files.graph.full);
        println("Landmarks backup : " + Files.landmarks.full);
        println("Result output    : " + Files.output.full);
        println("Compared output  : " + Files.comp_output.full);
        println("API icon         : " + Files.api_icon.full);

        return COMMAND;
    }
    if (input == "new port") {
        print("Enter a new port number (current is " + to_string(port) + ") : ");
        cin >> port;
        println("Port set to " + to_string(port) + "successfully", type::VALIDATION);
        kill_api.store(true);
        api_ready.store(false);
        thread(runApiServer).detach();
        return COMMAND;
    }
    if (input == "display api") {
        println("Which data do you want to (un)display ?");
        println(" 1/-1 : display/hide the valid requests");
        println(" 2/-2 : display/hide the bad   requests");
        println(" 3/-3 : display/hide the valid responses");
        println(" 4/-4 : display/hide the error responses");
        println(" 5/-5 : display/hide all the above");
        print("Option : ");

        int option;
        cin >> option;
        switch (option) {

        case 1:
            display_valid_endpoints = true;
            break;
        case -1:
            display_valid_endpoints = false;
            break;
        case 2:
            display_bad_endpoints = true;
            break;  
        case -2:
            display_bad_endpoints = false;
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
            display_valid_endpoints = true;
            display_bad_endpoints = true;
            display_valid_responses = true;
            display_error_responses = true;
            break;
        case -5:
            display_valid_endpoints = false;
            display_bad_endpoints = false;
            display_valid_responses = false;
            display_error_responses = false;
            break;
        default:
            println("Invalid option", type::WARNING);
            break;
        }

        println("Display options updated", type::VALIDATION);
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
    print("Enter the end node                : ");
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