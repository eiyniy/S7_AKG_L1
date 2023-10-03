#pragma once

#include <Matrix.hpp>
#include <vector>

class Vertex;

class CoordinateVector : public Matrix
{
public:
    CoordinateVector();
    ~CoordinateVector();
    CoordinateVector(const CoordinateVector &vector);
    CoordinateVector(const Vertex &vertex);
    CoordinateVector(const double v1, const double v2, const double v3, const double w = 0);

    CoordinateVector &operator*=(const CoordinateVector &cv);

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

    void convert(
        const CoordinateVector &xAxis,
        const CoordinateVector &yAxis,
        const CoordinateVector &zAxis,
        const CoordinateVector &translation);

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

    static Matrix getMoveConvert(
        const CoordinateVector &translation);

    static Matrix getConvertMatrix(
        const CoordinateVector &xAxis,
        const CoordinateVector &yAxis,
        const CoordinateVector &zAxis,
        const CoordinateVector &translation);

    static Matrix getObserverConvert(
        const CoordinateVector &eye,
        const CoordinateVector &target,
        const CoordinateVector &up);

    static Matrix getProjectionConvert(
        const double fov,
        const double aspect,
        const double zFar,
        const double zNear);

    static Matrix getWindowConvert(
        const double width,
        const double height,
        const double xMin,
        const double yMin);

    void log();
};

CoordinateVector operator*(const CoordinateVector &cv1, const CoordinateVector &cv2);
