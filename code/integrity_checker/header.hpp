
#ifndef HEADER_H
#define HEADER_H

#define CSV_LINES 28854312
#define NODE_MAX_VALUE 23947347
#define BACKUP "USA-roads_graph.bin"
#define DATASET "USA-roads.csv"
#define OUTPUT "shortest_path.csv"


#include <iostream>
#include <iomanip>
#include <vector>
#include <queue>
#include <limits>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>

// Alias
using namespace std;
using pii = pair<int, int>;

#include "outputs.hpp"
#include "graph.hpp"
#include "dag.hpp"

#endif