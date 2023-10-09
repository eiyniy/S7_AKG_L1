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
    const double &cGetValue(const int i, const int j) const;

    static Matrix<4, 4> getConvertMatrix(
        const CoordinateVector &xAxis,
        const CoordinateVector &yAxis,
        const CoordinateVector &zAxis,
        const CoordinateVector &translation);

    static Matrix<4, 4> getMoveConvert(
        const CoordinateVector &translation);

    static Matrix<4, 4> getScaleConvert(
        const CoordinateVector &scale);

    static Matrix<4, 4> getRotateConvert(
        const AxisName axis,
        const double angle);

    static Matrix<4, 4> getObserverConvert(
        const CoordinateVector &eye,
        const CoordinateVector &target,
        const CoordinateVector &up);

    static Matrix<4, 4> getProjectionConvert(
        const double fov,
        const double aspect,
        const double zFar,
        const double zNear);

    static Matrix<4, 4> getWindowConvert(
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

template <int Rows1, int Cols1, int Rows2, int Cols2>
Matrix<Rows1, Cols2> operator*(const Matrix<Rows1, Cols1> &m1, const Matrix<Rows2, Cols2> &m2);

template <int Rows, int Cols>
Matrix<Rows, Cols> operator*(const Matrix<Rows, Cols> &m, double v);

template <int Rows, int Cols>
Matrix<Rows, Cols> operator*(double v, const Matrix<Rows, Cols> &m);

template <int Rows, int Cols>
Matrix<Rows, Cols> operator/(const Matrix<Rows, Cols> &m, double v);

template <int Rows1, int Cols1, int Rows2, int Cols2>
Matrix<Rows1, Cols2> operator*(const Matrix<Rows1, Cols1> &m1, const Matrix<Rows2, Cols2> &m2)
{
    if (m1.cols != m2.rows)
        throw std::logic_error("Could not execute vector multiply");

    auto temp = Matrix<Rows1, Cols2>();

    for (int i = 0; i < temp.rows; ++i)
    {
        for (int k = 0; k < m1.cols; ++k)
        {
            for (int j = 0; j < temp.cols; ++j)
                temp.getValue(i, j) += (m1.cGetValue(i, k) * m2.cGetValue(k, j));
        }
    }

    return temp;
}

template <int Rows, int Cols>
Matrix<Rows, Cols> operator*(const Matrix<Rows, Cols> &m, double v)
{
    auto temp = Matrix<Rows, Cols>();

    for (int i = 0; i < m.rows; ++i)
    {
        for (int j = 0; j < m.cols; ++j)
            temp.getValue(i, j) = m.cGetValue(i, j) * v;
    }

    return temp;
}

template <int Rows, int Cols>
Matrix<Rows, Cols> operator+(const Matrix<Rows, Cols> &m1, const Matrix<Rows, Cols> &m2)
{
    if (m1.rows != m2.rows || m1.cols != m2.cols)
        throw std::logic_error("Could not execute + operator");

    auto temp = Matrix<Rows, Cols>();

    for (int i = 0; i < m1.rows; ++i)
    {
        for (int j = 0; j < m1.cols; ++j)
            temp.getValue(i, j) = m1.cGetValue(i, j) + m2.cGetValue(i, j);
    }

    return temp;
}
