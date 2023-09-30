#include <Math.hpp>

std::optional<int> Math::optStoi(const std::string &str)
{
    if (str.empty())
        return std::nullopt;
    else
        return stoi(str);
}