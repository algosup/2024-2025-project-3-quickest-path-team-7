/*

This program is a path finder using the Dijkstra algorithm.

The program reads a CSV file containing the nodes and their distances.
The CSV file should be formatted as follows:
node1, node2, distance

example:
1, 2, 10
2, 3, 20
3, 4, 30

To use a certain CSV file, you must adapt the 4 definitions at the beginning of the program:
- CSV_LINES: the number of lines in the CSV file
- NODE_MAX_VALUE: the maximum value of a node
- PATH: the name of the CSV file
- BACKUP: the name of the binary file to save the graph (try to keep the same name as the CSV file)

The program will then calculate the shortest path between two nodes using the Dijkstra algorithm.
The program will output the total distance, the calculation time,
as well as the nodes' list of this path to a CSV file named "shortest_path.csv".

The program will also save the graph to a binary file for future use.

To be known:
If you enter -1 as the start node, the program will force to reload the graph from the CSV file.
It is able to detect if an invalid node is entered.
It is able to detect if there is no path between the two nodes.

*/


#ifndef HEADER_H
#define HEADER_H

#define CSV_LINES 28854312
#define NODE_MAX_VALUE 23947347
#define BACKUP "USA-roads_graph.bin"
#define PATH "USA-roads.csv"


#include <iostream>
#include <vector>
#include <queue>
#include <limits>
#include <fstream>
#include <sstream>
#include <string>
#include <chrono>
#include <filesystem>


// Alias
using namespace std;
using pii = pair<int, int>;

#include "outputs.hpp"
#include "graph.hpp"
#include "dijkstra.hpp"


#endif