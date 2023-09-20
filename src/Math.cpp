#include <Math.hpp>

using namespace std;

optional<int> Math::optStoi(string str)
{
    if (str.empty())
        return nullopt;
    else
        return stoi(str);
}