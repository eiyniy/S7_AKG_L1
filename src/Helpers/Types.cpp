#include <Types.hpp>
#include <iostream>

Dot::Dot(const int p_x, const int p_y)
    : x(p_x), y(p_y) {}

SphericalCoordinate::SphericalCoordinate(
    const double p_r,
    const double p_a,
    const double p_b)
    : r(p_r), a(p_a), b(p_b) {}

void SphericalCoordinate::move(
    const AxisName axisName,
    const Direction direction,
    const double step,
    bool &isCameraReversed)
{
    static bool isANegative = false;
    double signedStep = isANegative ? -step : step;

    switch (axisName)
    {
    case AxisName::X:
        if (direction == Direction::Forward)
            b += step;
        else
            b -= step;
        break;
    case AxisName::Y:
        if (direction == Direction::Forward)
            a += signedStep;
        else
            a -= signedStep;
        break;
    }

    if (b < 0)
        b = 360;
    else if (b > 360)
        b = 0;

    if (a < 0 || a > 180)
    {
        b = b > 180 ? b - 180 : b + 180;
        isANegative = !isANegative;
        isCameraReversed = true;

        signedStep = isANegative ? -step : step;
        if (direction == Direction::Forward)
            a += signedStep;
        else
            a -= signedStep;

        std::cout << "A reversed" << std::endl;
    }
}
