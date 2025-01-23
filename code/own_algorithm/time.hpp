#ifndef TIME_H
#define TIME_H

#include "header.hpp"

struct Timer {
    chrono::high_resolution_clock::time_point start_time;
    chrono::high_resolution_clock::time_point end_time;
    long time;
};

void start_timer(Timer& timer) {
    timer.start_time = chrono::high_resolution_clock::now();
}

void stop_timer(Timer& timer) {
    timer.end_time = chrono::high_resolution_clock::now();
    timer.time = chrono::duration_cast<chrono::microseconds>(timer.end_time - timer.start_time).count();
}

#endif