#pragma once

#include <Enums.hpp>
#include <vector>
#include <stdexcept>
#include <optional>
#include <array>
#include <cmath>

//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// MATRIX [I] [J]: I->ROW; J->COL

template <int Rows, int Cols>
class Matrix
{
public:
    Matrix();
    Matrix(
        const double x,
        const double y,
        const double z,
        const double w = 1.f)
        requires(Rows == 4 && Cols == 1);

    Matrix(const Matrix &m);
    Matrix &operator=(const Matrix &m);

    Matrix &operator+=(const Matrix &m);
    Matrix &operator-=(const Matrix &m);
    Matrix<Rows, Cols> &operator*=(const Matrix<Rows, Cols> &m)
        requires(Rows == Cols);

    Matrix &operator+=(const double v);
    Matrix &operator-=(const double v);
    Matrix &operator*=(const double v);
    Matrix &operator/=(const double v);

    Matrix operator+(const Matrix &m) const;
    Matrix operator-(const Matrix &m) const;
    template <int Rows2, int Cols2>
    Matrix<Rows, Cols2> operator*(const Matrix<Rows2, Cols2> &m) const
        requires(Cols == Rows2);

    Matrix operator+(const double v) const;
    Matrix operator-(const double v) const;
    Matrix operator*(const double v) const;
    Matrix operator/(const double v) const;

    double &getValue(const int i, const int j);
    const double &cGetValue(const int i, const int j) const;

    const double cGetX() const
        requires(Rows == 4 && Cols == 1);
    const double cGetY() const
        requires(Rows == 4 && Cols == 1);
    const double cGetZ() const
        requires(Rows == 4 && Cols == 1);
    const double cGetW() const
        requires(Rows == 4 && Cols == 1);

    double &getX()
        requires(Rows == 4 && Cols == 1);
    double &getY()
        requires(Rows == 4 && Cols == 1);
    double &getZ()
        requires(Rows == 4 && Cols == 1);
    double &getW()
        requires(Rows == 4 && Cols == 1);

    const double scalarMultiply(const Matrix<4, 1> &vector) const
        requires(Rows == 4 && Cols == 1);
    const Matrix<4, 1> vectorMultiply(const Matrix<4, 1> &vector) const
        requires(Rows == 4 && Cols == 1);

    const double getLength()
        requires(Rows == 4 && Cols == 1);
    void normalize()
        requires(Rows == 4 && Cols == 1);

    static Matrix<4, 4> getConvertMatrix(
        const Matrix<4, 1> &xAxis,
        const Matrix<4, 1> &yAxis,
        const Matrix<4, 1> &zAxis,
        const Matrix<4, 1> &translation);

    static Matrix<4, 4> getMoveConvert(
        const Matrix<4, 1> &translation);

    static Matrix<4, 4> getScaleConvert(
        const Matrix<4, 1> &scale);

    static Matrix<4, 4> getRotateConvert(
        const AxisName axis,
        const double angle);

    static Matrix<4, 4> getViewConvert(
        const Matrix<4, 1> &eye,
        const Matrix<4, 1> &target,
        const Matrix<4, 1> &up);

    static Matrix<4, 4> getProjectionConvert(
        const double fov,
        const double aspect,
        const double zFar,
        const double zNear);

    static Matrix<4, 4> getViewportConvert(
        const double width,
        const double height,
        const double xMin,
        const double yMin);

    void log() const;

private:
    std::array<double, Cols * Rows> values;
    std::optional<double> length;
};

template <int Rows, int Cols>
inline double &Matrix<Rows, Cols>::getValue(const int i, const int j)
{
    return values[i * Cols + j];
}

template <int Rows, int Cols>
inline const double &Matrix<Rows, Cols>::cGetValue(const int i, const int j) const
{
    return values[i * Cols + j];
}

template <int Rows, int Cols>
inline const double Matrix<Rows, Cols>::cGetX() const
    requires(Rows == 4 && Cols == 1)
{
    return values[0];
}

template <int Rows, int Cols>
inline const double Matrix<Rows, Cols>::cGetY() const
    requires(Rows == 4 && Cols == 1)
{
    return values[1];
}

template <int Rows, int Cols>
inline const double Matrix<Rows, Cols>::cGetZ() const
    requires(Rows == 4 && Cols == 1)
{
    return values[2];
}

template <int Rows, int Cols>
inline const double Matrix<Rows, Cols>::cGetW() const
    requires(Rows == 4 && Cols == 1)
{
    return values[3];
}

template <int Rows, int Cols>
inline double &Matrix<Rows, Cols>::getX()
    requires(Rows == 4 && Cols == 1)
{
    return values[0];
}

template <int Rows, int Cols>
inline double &Matrix<Rows, Cols>::getY()
    requires(Rows == 4 && Cols == 1)
{
    return values[1];
}

template <int Rows, int Cols>
inline double &Matrix<Rows, Cols>::getZ()
    requires(Rows == 4 && Cols == 1)
{
    return values[2];
}

template <int Rows, int Cols>
inline double &Matrix<Rows, Cols>::getW()
    requires(Rows == 4 && Cols == 1)
{
    return values[3];
}
