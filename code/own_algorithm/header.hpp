#ifndef HEADER_H
#define HEADER_H

#define CSV_LINES           28854312
#define PORT                9500

#define ROOT_LANDMARK       1
#define LANDMARKS_QTY       10
// WEIGHT is the coefficient aplied to the heuristic cost for A star :
// Using 1 : provides the quickest path, but can take time to calculate
// Using 1.02 : provides a good balance between speed and accuracy (< 1% longer path than dijkstra)
#define WEIGHT              1.02

#define MAP_BACKUP          "map-backup.bin"
#define LANDMARKS_BACKUP    "landmarks-backup"
#define DATASET             "USA-roads.csv"
#define OUTPUT              "shortest-path.csv"

#define FAIL                false
#define SUCCESS             true
#define FORCE_BUILD         true
#define ASK_FOLDER          true
#define SKIP                false
#define COMMAND             -1
#define EXIT                -2
#define PATH                -3
#define INVALID_COMMAND     -4
#define INVALID_NODE        -5
#define NO_DATASET          -1
#define TIME_UNIT           microseconds
#define TIME_UNIT_STR       "µs"

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
#include <algorithm>
#include <locale>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>

// Alias
using namespace std;
using int_pair = pair<int, int>;

// Constants
const int INF = numeric_limits<int>::max();

// Includer
#include "globals.hpp"
#include "time.hpp"
#include "output.hpp"
#include "algorithms.hpp"
#include "landmarks.hpp"
#include "graph.hpp"
#include "input.hpp"
#include "api_responses.hpp"
#include "api_handling.hpp"

#endif
