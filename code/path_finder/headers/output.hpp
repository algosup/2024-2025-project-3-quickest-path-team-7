#ifndef OUTPUTS_H
#define OUTPUTS_H

#include "header.hpp"

enum class type {
    BASIC,
    ERROR,
    ERROR_LIGHT,
    LOG,
    VALIDATION,
    WARNING,
    INFO,
    BOLD,
    GREEN,
    BLUE,
    RED,
    YELLOW
};

void print(const string& message, type t = type::BASIC) {
    switch (t) {
        case type::BASIC:
        cout    << "\033[0m" // Normal color (white)
                << message
                << "\033[0m"
                << flush;
            break;
        case type::ERROR:
        cerr    << "\033[1;31m" // Bold red
                << message
                << "\033[0m"
                << flush;
            break;
        case type::ERROR_LIGHT:
        cerr    << "\033[31m" // Light red
                << message
                << "\033[0m"
                << flush;
            break;
        case type::LOG:
        cout    << "\033[34m" // light blue
                << message
                << "\033[0m"
                << flush;
            break;
        case type::VALIDATION: // bold green
        cout    << "\033[1;32m"
                << message
                << "\033[0m"
                << flush;
            break;
        case type::WARNING:
        cout    << "\033[1;33m" // Bold yellow
                << message
                << "\033[0m"
                << flush;
            break;
        case type::INFO:
        cout    << "\033[1;34m" // Bold blue
                << message
                << "\033[0m"
                << flush;
            break;
        case type::BOLD:
        cout    << "\033[1m" // Bold white
                << message
                << "\033[0m"
                << flush;
            break;
        case type::GREEN:
        cout    << "\033[32m" // light Green
                << message
                << "\033[0m"
                << flush;
            break;
        case type::BLUE:
        cout    << "\033[34m" // light blue
                << message
                << "\033[0m"
                << flush;
            break;
        case type::RED:
        cout    << "\033[31m" // light red
                << message
                << "\033[0m"
                << flush;
            break;
        case type::YELLOW:
        cout    << "\033[33m" // light yellow
                << message
                << "\033[0m"
                << flush;
            break;
    }
}

void print(const int message, type t = type::BASIC) {
    print(to_string(message), t);
}
void print (const float message, int precision, type t = type::BASIC) {
    stringstream ss;
    ss << fixed << setprecision(precision) << message;
    print(ss.str(), t);
}
void println(const string& message, type t = type::BASIC) {
    print(message + "\n", t);
}
void println(int message, type t = type::BASIC) {
    print(to_string(message) + "\n", t);
}
void println(float message, int precision, type t = type::BASIC) {
    stringstream ss;
    ss << fixed << setprecision(precision) << message;
    print(ss.str() + "\n", t);
}


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
        println("Failed to open the file for writing.", type::ERROR);
        return;
    }

    file << "Timestamp, "            << getCurrentTimestamp()                                               << "\n";
    file << "Dataset used, "         << GlobalFiles.dataset.base                                                << "\n";
    file << "A* ALT Algorithm, "     << "using " << landmarks_qty << " landmarks"                           << "\n";

    for (int i=0; i<GlobalGraph.landmarks.size(); i++) {
        file << "Landmark " << i+1 << ", " << formatWithSpaces(GlobalGraph.landmarks[i])                        << "\n";
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
        println("Failed to open the file for writing.", type::ERROR);
        return;
    }

    file << "Comparison of the Dijkstra and A* paths\n";
    file << "Time-stamp, "        << getCurrentTimestamp()                                                            << "\n";
    file << "Dataset used, "      << GlobalFiles.dataset.base                                                             << "\n";
    file << "A* ALT Algorithm, "  << "using " << landmarks_qty << " landmarks"                                        << "\n";

    for (int i=0; i<GlobalGraph.landmarks.size(); i++) {
        file << "Landmark " << i+1 << ", " << formatWithSpaces(GlobalGraph.landmarks[i])                                  << "\n";
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
        println("No path found between node " + formatWithSpaces(path_data.start) + " and node " + formatWithSpaces(path_data.end) + ".", type::ERROR);
        return;
    }

    // Output results
    println("Path lenght        : " + formatWithSpaces(path_data.distance));
    println("Number of nodes    : " + formatWithSpaces(path_data.path.size()));
    println("Calculation Time   : " + formatWithSpaces(path_data.calculation_time) + " " + TIME_UNIT_STR);
    println("Full info saved to : file://" + filesystem::absolute(GlobalFiles.output.full).string(), type::INFO);
}

void displayComparisonResults(Path& astar_path, Path& perfect_path) {
    // If there is no path between the two nodes, output a message
    if (astar_path.path.empty()) {
        println("No path found between node " + formatWithSpaces(astar_path.start) + " and node " + formatWithSpaces(astar_path.end) + ".", type::ERROR);
        return;
    }

    // Output results
    println("Comparison of the Dijkstra and A* paths");
    println("A* path is bigger by    : " + comparisonPercentage(astar_path.distance, perfect_path.distance) + "%");
    println("Dijkstra path length    : " + formatWithSpaces(perfect_path.distance));
    println("A* ALT   path length    : " + formatWithSpaces(astar_path.distance));
    println("Dijkstra nodes quantity : " + formatWithSpaces(perfect_path.path.size()));
    println("A* ALT   nodes quantity : " + formatWithSpaces(astar_path.path.size()));
    println("Dijkstra execution time : " + formatWithSpaces(perfect_path.calculation_time) + " " + TIME_UNIT_STR);
    println("A* ALT   execution time : " + formatWithSpaces(astar_path.calculation_time) + " " + TIME_UNIT_STR);
    println("Full info saved to      : file://" + filesystem::absolute(GlobalFiles.comp_output.full).string(), type::INFO);
}
#endif