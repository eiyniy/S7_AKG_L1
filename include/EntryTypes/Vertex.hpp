#pragma once

#include <BaseVertex.hpp>
#include <vector>
#include <string>
#include <optional>

class Vertex : public BaseVertex
{
public:
    Vertex(const double x, const double y, const double z, const double w = 1);

    static Vertex parse(const std::string &line);

    const double cGetX() const;
    const double cGetY() const;
    const double cGetZ() const;
    const double cGetW() const;

    double &getX();
    double &getY();
    double &getZ();
    double &getW();
};
