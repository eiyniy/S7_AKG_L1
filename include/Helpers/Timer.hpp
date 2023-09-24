#pragma once

#include <chrono>

class Timer
{
    static std::chrono::_V2::system_clock::time_point timeStart;
    static long long nsCount;
    static int callsCount;

public:
    static void start();
    static void stop();

    static long long getNs();
    static int getCalls();
};
