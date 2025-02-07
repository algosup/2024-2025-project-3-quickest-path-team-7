#ifndef TIME_H
#define TIME_H

#include "header.hpp"

// Function to get the current time as a string
string get_current_timestamp() {
    time_t now = time(0);
    tm *ltm = localtime(&now);
    char buffer[80];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", ltm);
    return string(buffer);
}

void start_timer(Timer& timer) {
    timer.start_time = chrono::high_resolution_clock::now();
}

void stop_timer(Timer& timer) {
    timer.end_time = chrono::high_resolution_clock::now();
    timer.time = chrono::duration_cast<chrono::TIME_UNIT>(timer.end_time - timer.start_time).count();
}

#endif