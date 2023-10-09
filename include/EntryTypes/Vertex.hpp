#pragma once

#include <BaseVertex.hpp>
#include <vector>
#include <string>
#include <optional>

class Vertex : public BaseVertex
{
public:
    Vertex();
    Vertex(const Vertex &v);

    Vertex &operator=(const Vertex &v);

    Vertex(const double x,
           const double y,
           const double z,
           const double w = 1,
           const bool p_isOutOfScreen = false,
           const bool p_isWNegative = false);

    static Vertex parse(const std::string &line);

    const bool cGetIsOutOfScreen() const;
    const bool cGetIsWNegative() const;

    const double cGetX() const;
    const double cGetY() const;
    const double cGetZ() const;
    const double cGetW() const;

    double &getX();
    double &getY();
    double &getZ();
    double &getW();

private:
    const bool isOutOfScreen;
    const bool isWNegative;
};
