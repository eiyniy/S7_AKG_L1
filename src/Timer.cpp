#include <Timer.hpp>

using namespace std;

chrono::_V2::system_clock::time_point Timer::timeStart;
long long Timer::nsCount;

void Timer::start()
{
    timeStart = chrono::high_resolution_clock::now();
}

void Timer::stop()
{
    auto timeEnd = chrono::high_resolution_clock::now();
    nsCount += chrono::duration_cast<chrono::nanoseconds>(timeEnd - timeStart).count();
}

long long Timer::getNs()
{
    return nsCount;
}
