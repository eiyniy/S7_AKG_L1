#pragma once

#include <BaseVertex.hpp>
#include <vector>
#include <string>
#include <optional>

class Vertex : public BaseVertex
{
public:
    Vertex(std::string &line);

    double getX() const;
    double getY() const;
    double getZ() const;
    std::optional<double> getW() const;
};
