#pragma once

#include <Types.hpp>
#include <vector>
#include <stdexcept>
#include <optional>
#include <array>
#include <cmath>

//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// MATRIX [I] [J]: I->ROW; J->COL

class CoordinateVector;

template <int Rows, int Cols>
class Matrix
{
public:
    Matrix();
    ~Matrix();

    Matrix(const Matrix &m);
    Matrix &operator=(const Matrix &m);

    double &getValue(const int i, const int j);

    static Matrix getConvertMatrix(
        const CoordinateVector &xAxis,
        const CoordinateVector &yAxis,
        const CoordinateVector &zAxis,
        const CoordinateVector &translation);

    static Matrix getMoveConvert(
        const CoordinateVector &translation);

    static Matrix getScaleConvert(
        const CoordinateVector &scale);

    static Matrix getRotateConvert(
        const AxisName axis,
        const double angle);

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

    const int rows, cols;

protected:
    std::array<std::array<double, Cols>, Rows> values;
    std::optional<double> length;
};

template <int Rows, int Cols>
Matrix<Rows, Cols> operator+(const Matrix<Rows, Cols> &m1, const Matrix<Rows, Cols> &m2);

template <int Rows, int Cols>
Matrix<Rows, Cols> operator-(const Matrix<Rows, Cols> &m1, const Matrix<Rows, Cols> &m2);

template <int Rows, int Cols>
Matrix<Rows, Cols> operator*(const Matrix<Rows, Cols> &m1, const Matrix<Rows, Cols> &m2);

template <int Rows, int Cols>
Matrix<Rows, Cols> operator*(const Matrix<Rows, Cols> &m, double v);

template <int Rows, int Cols>
Matrix<Rows, Cols> operator*(double v, const Matrix<Rows, Cols> &m);

template <int Rows, int Cols>
Matrix<Rows, Cols> operator/(const Matrix<Rows, Cols> &m, double v);
