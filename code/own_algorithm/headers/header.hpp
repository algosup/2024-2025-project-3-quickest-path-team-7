#ifndef HEADER_H
#define HEADER_H

#define CSV_LINES           28854312 // Serve only to display the loading progress in the console
#define WEIGHT              1.02
#define ROOT_LANDMARK       1 // The first landmark to be picked    
#define LANDMARKS_QTY       10
// WEIGHT is the coefficient aplied to the heuristic cost for A star :
// Using 1 : provides the quickest path, but can take time to calculate
// Using 1.02 : provides a good balance between speed and accuracy (< 1% longer path than dijkstra)

#define PORT                9500

#define MAP_BACKUP          "map-backup.bin"
#define LANDMARKS_BACKUP    "landmarks-backup"
#define DATASET             "USA-roads.csv"
#define OUTPUT              "shortest-path.csv"
#define API_ICON            "favicon.ico"
#define FOLDER              "files/"

#define LIGHT               false
#define FULL                true
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
#include <cstring>
#include <filesystem>

#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #pragma comment(lib, "ws2_32.lib")  // Link with Winsock library
    typedef int socklen_t; // Windows does not define socklen_t
#else
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <unistd.h>
#endif

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
#include "api_responses.hpp"
#include "api_handling.hpp"
#include "input.hpp"

#endif
