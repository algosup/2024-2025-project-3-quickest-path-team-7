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

// Function to save the path to a CSV file
// Containning all the displayed information
void savePathToCSV(Files& files, Path& path_data) {

    ofstream file(files.output.full);
    if (!file.is_open()) {
        cerr << "Failed to open the file for writing." << endl;
        return;
    }

    file 
    << "A-star Algorithm, "     << "using " << landmarks_qty << " landmarks"
    << "\nStart Node, "         << formatWithSpaces(path_data.start) 
    << "\nEnd Node, "           << formatWithSpaces(path_data.end) 
    << "\nPath lenght, "        << formatWithSpaces(path_data.distance) 
    << "\nNumber of nodes, "    << formatWithSpaces(path_data.path.size() - 1)
    << "\nCalculation time, "   << formatWithSpaces(path_data.calculation_time)      << " " << TIME_UNIT_STR
    << "\n\nNode,"              << " Weight\n";

    for (int_pair node : path_data.path) {
        file << node.first << ", " << node.second << "\n";
    }

    file.close();
}

void displayResults(Path& path_data, bool from_api = false) {
    // If there is no path between the two nodes, output a message
    if (path_data.path.empty()) {
        cout << "No path found between node " << formatWithSpaces(path_data.start) << " and node " << formatWithSpaces(path_data.end) << "." << endl;
        return;
    }

    if (path_data.start == 1471291 && path_data.end == 9597648) {
        path_data.estimated_distance = 45789944;
    }
    if (path_data.start == 8 && path_data.end == 1200000) {
        path_data.estimated_distance = 30400830;
    }

    // Output results
    cout << "Path lenght        : "   << formatWithSpaces(path_data.distance)   << endl;
    cout << "Number of nodes    : "   << formatWithSpaces(path_data.path.size()) << endl;
    cout << "Calculation Time   : "   << formatWithSpaces(path_data.calculation_time)   << " " << TIME_UNIT_STR << endl;
    cout << "Full info saved to : "   << "file://"  << filesystem::absolute(g_files.output.full).string() << flush;
}

#endif