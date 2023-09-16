#pragma once

#include <vector>
#include <optional>

class Vertex : std::vector<double>
{
public:
    void append(double value);

    double getX();
    double getY();
    double getZ();
    std::optional<double> getW();

    void print();
};
