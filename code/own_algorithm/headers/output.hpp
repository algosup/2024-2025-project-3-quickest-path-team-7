#ifndef OUTPUTS_H
#define OUTPUTS_H

#include "header.hpp"

// Function to format a number with spaces between each group of three digits
string formatWithSpaces(long number) {
    string numStr = to_string(number);
    int insertPosition = numStr.length() - 3;
    while (insertPosition > 0) {
        numStr.insert(insertPosition, " ");
        insertPosition -= 3;
    }
    return numStr;
}

string comparisonPercentage (int path_data_distance, int perfect_path_distance) {
    if (perfect_path_distance == 0) return "Best distance is 0 !";
    float percentage = (path_data_distance - perfect_path_distance) * 100 / perfect_path_distance;
    string percentage_str;
    stringstream ss;
    ss << fixed << setprecision(2) << percentage;
    ss >> percentage_str;
    return percentage_str;
}

// Function to save the path to a CSV file
// Containning all the displayed information
void savePathToCSV(Files& files, Path& path_data) {

    ofstream file(files.output.full);
    if (!file.is_open()) {
        cout << "Failed to open the file for writing." << endl;
        return;
    }

    file << "Timestamp, "            << get_current_timestamp()                                             << "\n";
    file << "Dataset used, "         << g_files.dataset.base                                                << "\n";
    file << "A* ALT Algorithm, "     << "using " << landmarks_qty << " landmarks"                           << "\n";;

    for (int i=0; i<g_graph.landmarks.size(); i++) {
        file << "Landmark " << i+1 << ", " << formatWithSpaces(g_graph.landmarks[i])                        << "\n";
    }

    file << "Start Node, "         << formatWithSpaces(path_data.start)                                     << "\n";
    file << "End Node, "           << formatWithSpaces(path_data.end)                                       << "\n";
    file << "Path lenght, "        << formatWithSpaces(path_data.distance)                                  << "\n";
    file << "Number of nodes, "    << formatWithSpaces(path_data.path.size())                               << "\n";
    file << "Calculation time, "   << formatWithSpaces(path_data.calculation_time)  << " " << TIME_UNIT_STR << "\n";
    file << "\nNode,"              << " Weight\n";

    for (int_pair node : path_data.path) {
        file << node.first << ", " << node.second                                                           << "\n";
    }

    file.close();
}

void saveComparedPathToCSV(Files& files, Path& astar_path, Path& perfect_path) {

    ofstream file(files.comp_output.full);
    if (!file.is_open()) {
        cout << "Failed to open the file for writing." << endl;
        return;
    }

    file << "Comparison of the Dijkstra and A* paths\n";
    file << "Time-stamp, "        << get_current_timestamp()                                                          << "\n";
    file << "Dataset used, "      << g_files.dataset.base                                                             << "\n";
    file << "A* ALT Algorithm, "  << "using " << landmarks_qty << " landmarks"                                        << "\n";

    for (int i=0; i<g_graph.landmarks.size(); i++) {
        file << "Landmark " << i+1 << ", " << formatWithSpaces(g_graph.landmarks[i])                                  << "\n";
    }

    file << "Start Node              , " << formatWithSpaces(astar_path.start)                                        << "\n";
    file << "End   Node              , " << formatWithSpaces(astar_path.end)                                          << "\n";
    file << "Dijkstra path length    , " << formatWithSpaces(astar_path.distance)                                     << "\n";
    file << "Astar    path length    , " << formatWithSpaces(perfect_path.distance)                                   << "\n";
    file << "Astar path is bigger by , " << comparisonPercentage(astar_path.distance, perfect_path.distance) << "%"   << "\n";
    file << "Dijkstra exec time      , " << formatWithSpaces(astar_path.calculation_time)    << " " << TIME_UNIT_STR  << "\n";
    file << "Astar exec time         , " << formatWithSpaces(perfect_path.calculation_time) << " " << TIME_UNIT_STR   << "\n";
    file << "\n";
    file << "ASTAR Node, DIJKSTRA Node, ASTAR Weight, DIJKSTRA Weight"                                                << "\n";
    
    for (int i = 0; i < astar_path.path.size(); i++) {
        file << astar_path.path[i].first     << ", ";
        file << perfect_path.path[i].first   << ", ";
        file << astar_path.path[i].second    << ", ";
        file << perfect_path.path[i].second  << "\n";
    }

    file.close();
}

void displayResults(Path& path_data) {
    // If there is no path between the two nodes, output a message
    if (path_data.path.empty()) {
        cout << "No path found between node " << formatWithSpaces(path_data.start) << " and node " << formatWithSpaces(path_data.end) << "." << endl;
        return;
    }

    // Output results
    cout << "Path lenght        : "   << formatWithSpaces(path_data.distance)   << endl;
    cout << "Number of nodes    : "   << formatWithSpaces(path_data.path.size()) << endl;
    cout << "Calculation Time   : "   << formatWithSpaces(path_data.calculation_time)   << " " << TIME_UNIT_STR << endl;
    cout << "Full info saved to : "   << "file://"  << filesystem::absolute(g_files.output.full).string() << endl;

}

void display_comparison_results(Path& astar_path, Path& perfect_path) {
    // If there is no path between the two nodes, output a message
    if (astar_path.path.empty()) {
        cout << "No path found between node " << formatWithSpaces(astar_path.start) << " and node " << formatWithSpaces(astar_path.end) << "." << endl;
        return;
    }

    // Output results
    cout << "Comparison of the Dijkstra and A* paths" << endl;
    cout << "A* path is bigger by    : " << comparisonPercentage(astar_path.distance, perfect_path.distance) << "%" << endl;
    cout << "Dijkstra path length    : " << formatWithSpaces(perfect_path.distance) << endl;
    cout << "A* ALT   path length    : " << formatWithSpaces(astar_path.distance) << endl;
    cout << "Dijkstra nodes quantity : " << formatWithSpaces(perfect_path.path.size()) << endl;
    cout << "A* ALT   nodes quantity : " << formatWithSpaces(astar_path.path.size()) << endl;
    cout << "Dijkstra execution time : " << formatWithSpaces(perfect_path.calculation_time) << " " << TIME_UNIT_STR << endl;
    cout << "A* ALT   execution time : " << formatWithSpaces(astar_path.calculation_time) << " " << TIME_UNIT_STR << endl;
    cout << "Full info saved to      : " << "file://" << filesystem::absolute(g_files.comp_output.full).string() << endl;
}
#endif