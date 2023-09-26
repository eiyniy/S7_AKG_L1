#pragma once

#include <Matrix.hpp>
#include <vector>

class CoordinateVector : public Matrix
{
public:
    CoordinateVector();
    CoordinateVector(const CoordinateVector &vector);
    CoordinateVector(double v1, double v2, double v3, double p_w = 0);

    double getX();
    double getY();
    double getZ();
    double getW();

    double scalarMultiply(const CoordinateVector &vector);
    const double getLength();
    CoordinateVector getNormalized();

private:
    double w;
};
