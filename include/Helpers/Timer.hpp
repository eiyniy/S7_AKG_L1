#pragma once

#include <chrono>

class Timer {
    static std::chrono::time_point<std::chrono::high_resolution_clock> timeStart;
    static long long nsCount;
    static long long callsCount;

public:
    static void start();

    static void stop();

    static void reset();

    static long long getMcs();

    static long long getCalls();
};
