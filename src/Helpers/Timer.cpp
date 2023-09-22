#include <Timer.hpp>

std::chrono::_V2::system_clock::time_point Timer::timeStart;
long long Timer::nsCount;

void Timer::start()
{
    timeStart = std::chrono::high_resolution_clock::now();
}

void Timer::stop()
{
    auto timeEnd = std::chrono::high_resolution_clock::now();
    nsCount += std::chrono::duration_cast<std::chrono::nanoseconds>(timeEnd - timeStart).count();
}

long long Timer::getNs()
{
    return nsCount;
}