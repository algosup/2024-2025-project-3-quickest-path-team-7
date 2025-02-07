#ifndef TIME_H
#define TIME_H

#include "header.hpp"

// Get the current time and date for the CSV export of path
string getCurrentTimestamp() {
    time_t now = time(0);
    tm *ltm = localtime(&now);
    char buffer[80];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", ltm);
    return string(buffer);
}

void startTimer(Timer& Timer) {
    Timer.start_time = chrono::high_resolution_clock::now();
}

void stopTimer(Timer& Timer) {
    Timer.end_time = chrono::high_resolution_clock::now();
    Timer.time = chrono::duration_cast<chrono::TIME_UNIT>(Timer.end_time - Timer.start_time).count();
}

#endif