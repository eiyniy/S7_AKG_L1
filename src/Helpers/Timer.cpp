#include <Timer.hpp>

std::chrono::time_point<std::chrono::high_resolution_clock> Timer::timeStart;
long long Timer::nsCount;
long long Timer::callsCount;

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

const long long Timer::getMcs()
{
    return nsCount / 1000;
}

const int Timer::getCalls()
{
    return callsCount;
}
