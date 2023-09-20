#pragma once

#include <string>
#include <vector>
#include <optional>

class BaseVertex
{
public:
    BaseVertex(std::string &line);

protected:
    double v1;
    std::optional<double> v2;
    std::optional<double> v3;
    std::optional<double> v4;
};
