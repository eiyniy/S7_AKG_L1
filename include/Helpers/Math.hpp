#pragma once

#include <optional>
#include <string>

class Math
{
public:
    Math() = delete;

    static std::optional<int> optStoi(const std::string &str);
};
