#include <Timer.hpp>

std::chrono::time_point<std::chrono::high_resolution_clock> Timer::timeStart;
long long Timer::nsCount;
int Timer::callsCount;

void Timer::start()
{
    timeStart = std::chrono::high_resolution_clock::now();
}

void Timer::stop()
{
    auto timeEnd = std::chrono::high_resolution_clock::now();

    nsCount += std::chrono::duration_cast<std::chrono::nanoseconds>(timeEnd - timeStart).count();
    ++callsCount;
}

const long long Timer::getNs()
{
    return nsCount;
}

const int Timer::getCalls()
{
    return callsCount;
}
