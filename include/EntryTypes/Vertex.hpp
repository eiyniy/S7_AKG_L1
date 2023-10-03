#pragma once

#include <BaseVertex.hpp>
#include <vector>
#include <string>
#include <optional>

class Vertex : public BaseVertex
{
public:
    Vertex(const std::string &line);
    Vertex(const BaseVertex &bVertex);

    const double cGetX() const;
    const double cGetY() const;
    const double cGetZ() const;
    const double cGetW() const;

    double &getX();
    double &getY();
    double &getZ();
    double &getW();
};
