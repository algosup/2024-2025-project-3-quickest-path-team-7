#ifndef HEADER_H
#define HEADER_H

#define CSV_LINES           28854312
#define NODE_MAX_VALUE      23947347

#define ROOT                1
#define LANDMARKS_QTY       10
// WEIGHT is the coefficient aplied to the heuristic cost for A star :
// Using 1 : provides the quickest path, but can take time to calculate
// Using 1.1 : provides a good balance between speed and accuracy (< 5% longer path)
#define WEIGHT              1.1

#define MAP_BACKUP          "map-backup.bin"
#define LANDMARKS_BACKUP    "landmarks-backup"
#define DATASET             "USA-roads.csv"
#define OUTPUT              "shortest-path.csv"

#define INVALID_NODE        -1
#define STOP                -2

#define TIME_UNIT microseconds
#define TIME_UNIT_STR "µs"

#define ASK_FOLDER          false
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
#include <iomanip>
#include <thread>
#include <mutex>
#include <atomic>

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
