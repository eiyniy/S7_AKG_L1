#include <Timer.hpp>

using namespace std;

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
