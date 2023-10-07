#pragma once

#include <Matrix.hpp>
#include <vector>

class Vertex;

class CoordinateVector : public Matrix<4, 1>
{
public:
    CoordinateVector();
    CoordinateVector(const CoordinateVector &vector);
    CoordinateVector(const double v1, const double v2, const double v3, const double w = 0);

    CoordinateVector &operator=(const CoordinateVector &cv);

    const double cGetX() const;
    const double cGetY() const;
    const double cGetZ() const;
    const double cGetW() const;

    double &getX();
    double &getY();
    double &getZ();
    double &getW();

    double scalarMultiply(const CoordinateVector &vector);
    const double getLength();
    CoordinateVector getNormalized();

    void log() const;
};

CoordinateVector operator+(const CoordinateVector &cv1, const CoordinateVector &cv2);
CoordinateVector operator*(const CoordinateVector &cv1, const CoordinateVector &cv2);
CoordinateVector operator/(const CoordinateVector &cv1, const double v);
