#ifndef TIME_H
#define TIME_H

#include "header.hpp"

void start_timer(Timer& timer) {
    timer.start_time = chrono::high_resolution_clock::now();
}

void stop_timer(Timer& timer) {
    timer.end_time = chrono::high_resolution_clock::now();
    timer.time = chrono::duration_cast<chrono::TIME_UNIT>(timer.end_time - timer.start_time).count();
}

#endif