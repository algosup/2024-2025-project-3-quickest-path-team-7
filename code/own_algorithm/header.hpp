#ifndef HEADER_H
#define HEADER_H

#define CSV_LINES 28854312
#define NODE_MAX_VALUE 23947347

#define ROOT 1
#define LANDMARKS 10

#define BACKUP "graph-backup.bin"
#define DATASET "USA-roads.csv"
#define OUTPUT "shortest-path.csv"

#define ASK_FOLDER false
// if true, the program will ask, if wanted, for a different folder path than the working directory 
// for input and output files

#include <iostream>
#include <vector>
#include <queue>
#include <limits>
#include <fstream>
#include <sstream>
#include <string>
#include <chrono>
#include <unordered_set>
#include <cmath>

// Alias
using namespace std;
using int_pair = pair<int, int>;

// Constants
const int INF = numeric_limits<int>::max();

// Includer
#include "time.hpp"
#include "output.hpp"
#include "algorithms.hpp"
#include "landmarks.hpp"
#include "graph.hpp"
#include "input.hpp"

#endif
