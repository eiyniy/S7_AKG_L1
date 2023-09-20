#include "Math.hpp"

using namespace std;

std::optional<int> Math::optStoi(std::string str)
{
    if (str.empty())
        return nullopt;
    else
        return stoi(str);
}