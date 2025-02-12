#ifndef HEADER_H
#define HEADER_H

#define CSV_LINES           28854312    // Serve only to display the loading progress in the console
#define WEIGHT              1.02        // The weight of the heuristic cost for A star
#define ROOT_LANDMARK       1           // The first landmark to be picked    
#define LANDMARKS_QTY       10          // The number of landmarks to be used

#define PORT                9500
#define DISP_BAD_EPT        false       // Display bad endpoints requests in terminal
#define DISP_VALID_EPT      false       // Display valid endpoints requests in terminal
#define DISP_VALID_RES      false       // Display valid responses in terminal
#define DISP_ERR_RES        true        // Display error responses in terminal

// Choose where is the CSV for dataset 
#define FOLDER              "files"
#define DATASET             "USA-roads.csv"

#define GRAPH_BACKUP        "-graph-backup"
#define LANDMARKS_BACKUP    "-X-landmarks-backup"
#define OUTPUT              "shortest-path"
#define COMP_OUTPUT         "compared-shortest-path"
#define API_ICON            "favicon.ico"
#define TEST_RESULTS        "sample-X-random-results"

#define TIME_UNIT           microseconds
#define TIME_UNIT_FULL_STR  "microseconds"
#define TIME_UNIT_STR       "µs"

// Encoding of options and return codes
#define LIGHT               1
#define DEBUG               2
#define COMPARE             3
#define INVALID_PARAMETER   1
#define MISSING_PARAMETER   2
#define NODE_NOT_FOUND      3
#define NODE_OUT_OF_RANGE   4
#define SAME_NODE           5
#define COMMAND             -1
#define EXIT                -2
#define PATH                -3
#define INVALID_COMMAND     -4
#define INVALID_NODE        -5
#define NO_DATASET          -1
#define NULL_HEURISTIC      true
#define FORCE_BUILD         true
#define ASK_FOLDER          true
#define NEW_ONE             true
#define SUCCESS             true
#define FAIL                false
#define SKIP                false

// Libraries includes
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
#include <ctime>

// API includes
#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #include <windows.h>
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

// Includer of all code files
#include "globals.hpp"
#include "time.hpp"
#include "output.hpp"
#include "files.hpp"
#include "algorithms.hpp"
#include "landmarks.hpp"
#include "graph.hpp"
#include "api_responses.hpp"
#include "api_handling.hpp"
#include "random_tester.hpp"
#include "menu.hpp"

#endif
