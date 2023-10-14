#pragma once

#include <Enums.hpp>

class SphericalCoordinate
{
public:
    SphericalCoordinate(
        const double p_r,
        const double p_a,
        const double p_b);

    void move(
        const AxisName axisName,
        const Direction direction,
        const double step,
        bool &isCameraReversed);

    double r, a, b;
};