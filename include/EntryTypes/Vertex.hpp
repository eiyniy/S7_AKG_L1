#pragma once

#include <BaseVertex.hpp>
#include <Types.hpp>
#include <vector>
#include <string>
#include <optional>

class Vertex : public BaseVertex
{
public:
    Vertex(std::string &line, ParseType parseType);

    double getX();
    double getY();
    double getZ();
    std::optional<double> getW();
};
