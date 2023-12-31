#pragma once

#include <Enums.hpp>

class SphericalCoordinate
{
public:
    SphericalCoordinate(
        const double _r,
        const double _a,
        const double _b);

    void move(
        const AxisName axisName,
        const Direction direction,
        const double step,
        bool &isCameraReversed);

    double r, a, b;
};