#ifndef INPUT_HPP
#define INPUT_HPP

#include "header.hpp"

int takeUserInput(PathData& path_data, vector<vector<pii>>& graph, bool& break_early) {

    // Ask and check for the start and end nodes to calculate the shortest path
    cout << "\n\nEnter the start node: ";
    cin >> path_data.start;
    if (path_data.start == -1) {
        loadGraph(graph, true);
        return -1; // Restart the loop
    }
    if (path_data.start < 0 || path_data.start >= graph.size() || graph[path_data.start].empty()) 
    {
        cout << "Invalid node. Please try again." << endl;
        return -1;
    }
    cout << "Enter the end node: ";
    cin >> path_data.end;
    if (path_data.end < 0 || path_data.end >= graph.size() || graph[path_data.end].empty())
    {
        cout << "Invalid node. Please try again." << endl;
        return -1;
    }

    // Ask the user if they want to break early the dijkstra algorithm
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

#endif