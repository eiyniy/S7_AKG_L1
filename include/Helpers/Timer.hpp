#pragma once

#include <chrono>

class Timer
{
    static std::chrono::time_point<std::chrono::high_resolution_clock> timeStart;
    static long long nsCount;
    static int callsCount;

public:
    static void start();
    static void stop();

    static const long long getNs();
    static const int getCalls();
};
