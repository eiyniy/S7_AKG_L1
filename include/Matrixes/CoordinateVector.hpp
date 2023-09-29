#pragma once

#include <Matrix.hpp>
#include <vector>

class Vertex;

class CoordinateVector : public Matrix
{
public:
    CoordinateVector();
    CoordinateVector(const CoordinateVector &vector);
    CoordinateVector(const Vertex &vertex);
    CoordinateVector(double v1, double v2, double v3, double w = 0);

    static CoordinateVector fromCoordinats(double v1, double v2, double v3, double w);

    CoordinateVector &operator*=(const CoordinateVector &cv);

    double getX() const;
    double getY() const;
    double getZ() const;
    double getW() const;

    double scalarMultiply(const CoordinateVector &vector);
    const double getLength();
    CoordinateVector getNormalized();

    void convert(
        const CoordinateVector xAxis,
        const CoordinateVector yAxis,
        const CoordinateVector zAxis,
        const CoordinateVector translation);

    void moveConvert(
        const CoordinateVector &translation);

    void scaleConvert(
        const CoordinateVector &scale);

    void rotateConvert(
        const AxisName axis,
        const double angle);

    void toObserverConvert(
        const CoordinateVector &eye,
        const CoordinateVector &target,
        const CoordinateVector &up);

    void toProjectionConvert(
        const double fov,
        const double aspect,
        const double zFar,
        const double zNear);

    void toViewerConvert(
        const double width,
        const double height,
        const double xMin,
        const double yMin);

    void log();
};

CoordinateVector operator*(const CoordinateVector &cv1, const CoordinateVector &cv2);
