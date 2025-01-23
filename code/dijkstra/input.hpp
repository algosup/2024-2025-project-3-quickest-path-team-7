#ifndef INPUT_HPP
#define INPUT_HPP

#include "header.hpp"

int takeUserInput(PathData& path_data, Graph& graph, Files& files, bool& break_early) {

    // Ask and check for the start and end nodes to calculate the shortest path
    cout << "\n\nEnter the start node: ";
    cin >> path_data.start;
    if (path_data.start == -1) {
        buildGraph(graph, files, true);
        return -1; // Restart the loop
    }
    if (path_data.start < 0 || path_data.start >= graph.data.size() || graph.data[path_data.start].empty()) 
    {
        cout << "Invalid node. Please try again." << endl;
        return -1;
    }
    cout << "Enter the end node: ";
    cin >> path_data.end;
    if (path_data.end < 0 || path_data.end >= graph.data.size() || graph.data[path_data.end].empty())
    {
        cout << "Invalid node. Please try again." << endl;
        return -1;
    }

    // Ask the user if they want to break early the dijkstra algorithm

    if (FORCE_BREAK_EARLY) {
        break_early = true;
        return 0;
    }

    char c = 'x';
    while (c!='y' && c!='n') {
        cout << "Break early (y/n)? ";
        cin >> c;
        if (c == 'y') {
            break_early = true;
        } else {
            if (c == 'n') {
                break_early = false;
            } else {
                cout << "Invalid input. Please try again." << endl;
            }
        }
    } 

    return 0;
}

void askComparison(int precise_distance)
{
    
    string input;
    cout << "\nEnter the distance to compare: " << flush;
    cin.ignore();
    getline(cin, input);
    // Remove spaces between digits from input
    input.erase(remove(input.begin(), input.end(), ' '), input.end());
    // Convert to integer
    int heuristic_distance = stoi(input);

    if (0 <= heuristic_distance <= precise_distance) {
        // Calculate how many percents more the heuristic distance is compared to the precise distance
        double percent = ((double)heuristic_distance / (double)precise_distance - 1) * 100;
        cout << "This distance is " << fixed << setprecision(2) << percent << "% bigger than the optimal one." << endl;
    } else {
        cout << "Invalid distance. Please try again." << endl;
        askComparison(precise_distance);
    }
}

void takeFolderInput(Files& files, bool ask_folder) {

    if (ask_folder){
        char c = 'x';
        while (c!='y' && c!='n') {
            cout << "Do you want to change the folder for input/output files ? (y/n)" << flush;
            cin >> c;
            if (c == 'y') {
                cout << "\nPlease provide the folder path: ";
                cin >> files.folder_path;
                files.dataset = files.folder_path + "/" + DATASET;
                files.output = files.folder_path + "/" + OUTPUT;
                files.backup = files.folder_path + "/" + BACKUP;
            }
            if (c == 'n') {
                files.folder_path = "";
                files.dataset = DATASET;
                files.output = OUTPUT;
                files.backup = BACKUP;
            } 
            if (c != 'y' && c != 'n') {
                cout << "Invalid input. Please try again." << endl;
            }
        }
    } else {
        files.folder_path = "";
        files.dataset = DATASET;
        files.output = OUTPUT;
        files.backup = BACKUP;
    }
    
    cout << "\nThe program will read this dataset: " << files.dataset << endl;
    cout << "The program will save the output to this file: " << files.output << endl;
    cout << "The program will save a backup of the graph to this file: " << files.backup << endl;
}

#endif