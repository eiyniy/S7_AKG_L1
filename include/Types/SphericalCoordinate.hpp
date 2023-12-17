#pragma once

#include <Enums.hpp>

class SphericalCoordinate {
public:
    SphericalCoordinate(
            double _r,
            double _a,
            double _b);

    void move(
            AxisName axisName,
            Direction direction,
            double step,
            bool &isCameraReversed);

    double r, a, b;
};