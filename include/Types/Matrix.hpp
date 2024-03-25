#pragma once

#include <Types.hpp>
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
        double x,
        double y,
        double z,
        double w = 1.f)
        requires(Rows == 4 && Cols == 1);

    Matrix(std::initializer_list<double> initList);

    Matrix(const Matrix &m);

    Matrix &operator=(const Matrix &m);

    Matrix &operator+=(const Matrix &m);

    Matrix &operator-=(const Matrix &m);

    Matrix<Rows, Cols> &operator*=(const Matrix<Rows, Cols> &m)
        requires(Rows == Cols);

    Matrix &operator+=(double v);

    Matrix &operator-=(double v);

    Matrix &operator*=(double v);

    Matrix &operator/=(double v);

    Matrix operator+(const Matrix &m) const;

    Matrix operator-(const Matrix &m) const;

    template <int Rows2, int Cols2>
    Matrix<Rows, Cols2> operator*(const Matrix<Rows2, Cols2> &m) const
        requires(Cols == Rows2);

    Matrix operator+(double v) const;

    Matrix operator-(double v) const;

    Matrix operator*(double v) const;

    Matrix operator/(double v) const;

    double &getValue(int i, int j);

    const double &cGetValue(int i, int j) const;

    double cGetX() const
        requires(Rows >= 1 && Cols == 1);

    double cGetY() const
        requires(Rows >= 2 && Cols == 1);

    double cGetZ() const
        requires(Rows >= 3 && Cols == 1);

    double cGetW() const
        requires(Rows >= 4 && Cols == 1);

    double &getX()
        requires(Rows >= 1 && Cols == 1);

    double &getY()
        requires(Rows >= 2 && Cols == 1);

    double &getZ()
        requires(Rows >= 3 && Cols == 1);

    double &getW()
        requires(Rows >= 4 && Cols == 1);

    double scalarMultiply(const Matrix<4, 1> &vector) const
        requires(Rows == 4 && Cols == 1);

    Matrix<4, 1> vectorMultiply(const Matrix<4, 1> &vector) const
        requires(Rows == 4 && Cols == 1);

    double getLength() const
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
        AxisName axis,
        double angle);

    static Matrix<4, 4> getViewConvert(
        const Matrix<4, 1> &eye,
        const Matrix<4, 1> &target,
        const Matrix<4, 1> &up);

    static Matrix<4, 4> getProjectionConvert(
        double fov,
        double aspect,
        double zFar,
        double zNear);

    static Matrix<4, 4> getViewportConvert(
        double width,
        double height,
        double xMin,
        double yMin);

    void log() const;

private:
    std::array<double, Cols * Rows> values;
    mutable std::optional<double> length;
};

template <int Rows, int Cols>
inline Matrix<Rows, Cols>::Matrix(std::initializer_list<double> initList)
{
    int i = 0;
    for (auto iter = initList.begin(); iter < initList.end(); ++i, ++iter)
        values[i] = *iter;
}

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
inline double Matrix<Rows, Cols>::cGetX() const
    requires(Rows >= 1 && Cols == 1)
{
    return values[0];
}

template <int Rows, int Cols>
inline double Matrix<Rows, Cols>::cGetY() const
    requires(Rows >= 2 && Cols == 1)
{
    return values[1];
}

template <int Rows, int Cols>
inline double Matrix<Rows, Cols>::cGetZ() const
    requires(Rows >= 3 && Cols == 1)
{
    return values[2];
}

template <int Rows, int Cols>
inline double Matrix<Rows, Cols>::cGetW() const
    requires(Rows >= 4 && Cols == 1)
{
    return values[3];
}

template <int Rows, int Cols>
inline double &Matrix<Rows, Cols>::getX()
    requires(Rows >= 1 && Cols == 1)
{
    return values[0];
}

template <int Rows, int Cols>
inline double &Matrix<Rows, Cols>::getY()
    requires(Rows >= 2 && Cols == 1)
{
    return values[1];
}

template <int Rows, int Cols>
inline double &Matrix<Rows, Cols>::getZ()
    requires(Rows >= 3 && Cols == 1)
{
    return values[2];
}

template <int Rows, int Cols>
inline double &Matrix<Rows, Cols>::getW()
    requires(Rows >= 4 && Cols == 1)
{
    return values[3];
}

template <int Rows>
using Vector = Matrix<Rows, 1>;
