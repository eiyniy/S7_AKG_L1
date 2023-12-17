#include <SphericalCoordinate.hpp>

SphericalCoordinate::SphericalCoordinate(
        const double _r,
        const double _a,
        const double _b)
        : r(_r), a(_a), b(_b) {}

void SphericalCoordinate::move(
        const AxisName axisName,
        const Direction direction,
        const double step,
        bool &isCameraReversed) {
    static bool isANegative = false;
    double signedStep = isANegative ? -step : step;

    switch (axisName) {
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
        case AxisName::Z:
            break;
    }

    if (b < 0)
        b = 360;
    else if (b > 360)
        b = 0;

    if (a < 0 || a > 180) {
        b = b > 180 ? b - 180 : b + 180;
        isANegative = !isANegative;
        isCameraReversed = true;

        signedStep = isANegative ? -step : step;
        if (direction == Direction::Forward)
            a += signedStep;
        else
            a -= signedStep;
    }
}