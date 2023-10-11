#include <iostream>
#include <cmath>
#include <Matrix.hpp>
#include <Converter.hpp>

#pragma region CONSTRUCTION

template <int Rows, int Cols>
Matrix<Rows, Cols>::Matrix()
{
    for (int i = 0; i < Rows; ++i)
    {
        for (int j = 0; j < Cols; ++j)
            values[i][j] = 0;
    }
}

template <int Rows, int Cols>
Matrix<Rows, Cols>::Matrix(
    const double x,
    const double y,
    const double z,
    const double w)
    requires(Rows == 4 && Cols == 1)
{
    values[0][0] = x;
    values[1][0] = y;
    values[2][0] = z;
    values[3][0] = w;
};

template <int Rows, int Cols>
Matrix<Rows, Cols>::~Matrix()
{
}

// TODO: FIX STRORAGE ALLOCATION
template <int Rows, int Cols>
Matrix<Rows, Cols>::Matrix(const Matrix &m)
{
    values = std::array<std::array<double, Cols>, Rows>();

    for (int i = 0; i < Rows; ++i)
    {
        for (int j = 0; j < Cols; ++j)
            values[i][j] = m.values[i][j];
    }
}

#pragma endregion CONSTRUCTION

#pragma region OPERATORS

template <int Rows, int Cols>
Matrix<Rows, Cols> &Matrix<Rows, Cols>::operator=(const Matrix &m)
{
    if (this == &m)
        return *this;

    for (int i = 0; i < Rows; ++i)
    {
        for (int j = 0; j < Cols; ++j)
            values[i][j] = m.values[i][j];
    }

    return *this;
}

template <int Rows, int Cols>
Matrix<Rows, Cols> &Matrix<Rows, Cols>::operator+=(const Matrix &m)
{
    for (int i = 0; i < Rows; ++i)
    {
        for (int j = 0; j < Cols; ++j)
            values[i][j] += m.values[i][j];
    }

    return *this;
}

template <int Rows, int Cols>
Matrix<Rows, Cols> &Matrix<Rows, Cols>::operator-=(const Matrix &m)
{
    for (int i = 0; i < Rows; ++i)
    {
        for (int j = 0; j < Cols; ++j)
            values[i][j] -= m.values[i][j];
    }

    return *this;
}

template <int Rows, int Cols>
Matrix<Rows, Cols> &Matrix<Rows, Cols>::operator*=(const Matrix<Rows, Cols> &m)
    requires(Rows == Cols)
{
    Matrix<Rows, Cols> temp = *this * m;
    return (*this = temp);
}

template <int Rows, int Cols>
Matrix<Rows, Cols> &Matrix<Rows, Cols>::operator+=(const double v)
{
    for (int i = 0; i < Rows; ++i)
    {
        for (int j = 0; j < Cols; ++j)
            values[i][j] += v;
    }

    return *this;
}

template <int Rows, int Cols>
Matrix<Rows, Cols> &Matrix<Rows, Cols>::operator-=(const double v)
{
    for (int i = 0; i < Rows; ++i)
    {
        for (int j = 0; j < Cols; ++j)
            values[i][j] -= v;
    }

    return *this;
}

template <int Rows, int Cols>
Matrix<Rows, Cols> &Matrix<Rows, Cols>::operator*=(const double v)
{
    for (int i = 0; i < Rows; ++i)
    {
        for (int j = 0; j < Cols; ++j)
            values[i][j] *= v;
    }

    return *this;
}

template <int Rows, int Cols>
Matrix<Rows, Cols> &Matrix<Rows, Cols>::operator/=(const double v)
{
    for (int i = 0; i < Rows; ++i)
    {
        for (int j = 0; j < Cols; ++j)
            values[i][j] /= v;
    }

    return *this;
}

template <int Rows, int Cols>
Matrix<Rows, Cols> Matrix<Rows, Cols>::operator+(const Matrix<Rows, Cols> &m) const
{
    auto temp = Matrix<Rows, Cols>(*this);
    return (temp += m);
}

template <int Rows, int Cols>
Matrix<Rows, Cols> Matrix<Rows, Cols>::operator-(const Matrix<Rows, Cols> &m) const
{
    auto temp = Matrix<Rows, Cols>(*this);
    return (temp -= m);
}

template <int Rows, int Cols>
Matrix<Rows, Cols> Matrix<Rows, Cols>::operator+(const double v) const
{
    auto temp = Matrix<Rows, Cols>(*this);
    return (temp += v);
}

template <int Rows, int Cols>
Matrix<Rows, Cols> Matrix<Rows, Cols>::operator-(const double v) const
{
    auto temp = Matrix<Rows, Cols>(*this);
    return (temp -= v);
}

template <int Rows, int Cols>
Matrix<Rows, Cols> Matrix<Rows, Cols>::operator*(const double v) const
{
    auto temp = Matrix<Rows, Cols>(*this);
    return (temp *= v);
}

template <int Rows, int Cols>
Matrix<Rows, Cols> Matrix<Rows, Cols>::operator/(const double v) const
{
    auto temp = Matrix<Rows, Cols>(*this);
    return (temp /= v);
}

template <int Rows, int Cols>
template <int Rows2, int Cols2>
Matrix<Rows, Cols2> Matrix<Rows, Cols>::operator*(const Matrix<Rows2, Cols2> &m) const
    requires(Cols == Rows2)
{
    auto temp = Matrix<Rows, Cols2>();

    for (int i = 0; i < Rows; ++i)
    {
        for (int k = 0; k < Cols; ++k)
        {
            for (int j = 0; j < Cols2; ++j)
                temp.getValue(i, j) += (values[i][k] * m.cGetValue(k, j));
        }
    }

    return temp;
}

#pragma endregion OPERATORS

#pragma region FUNCTIONS

template <int Rows, int Cols>
double &Matrix<Rows, Cols>::getValue(const int i, const int j)
{
    return values[i][j];
}

template <int Rows, int Cols>
const double &Matrix<Rows, Cols>::cGetValue(const int i, const int j) const
{
    return values[i][j];
}

template <int Rows, int Cols>
const double Matrix<Rows, Cols>::cGetX() const
    requires(Rows == 4 && Cols == 1)
{
    return values[0][0];
}

template <int Rows, int Cols>
const double Matrix<Rows, Cols>::cGetY() const
    requires(Rows == 4 && Cols == 1)
{
    return values[1][0];
}

template <int Rows, int Cols>
const double Matrix<Rows, Cols>::cGetZ() const
    requires(Rows == 4 && Cols == 1)
{
    return values[2][0];
}

template <int Rows, int Cols>
const double Matrix<Rows, Cols>::cGetW() const
    requires(Rows == 4 && Cols == 1)
{
    return values[3][0];
}

template <int Rows, int Cols>
double &Matrix<Rows, Cols>::getX()
    requires(Rows == 4 && Cols == 1)
{
    return values[0][0];
}

template <int Rows, int Cols>
double &Matrix<Rows, Cols>::getY()
    requires(Rows == 4 && Cols == 1)
{
    return values[1][0];
}

template <int Rows, int Cols>
double &Matrix<Rows, Cols>::getZ()
    requires(Rows == 4 && Cols == 1)
{
    return values[2][0];
}

template <int Rows, int Cols>
double &Matrix<Rows, Cols>::getW()
    requires(Rows == 4 && Cols == 1)
{
    return values[3][0];
}

template <int Rows, int Cols>
const double Matrix<Rows, Cols>::scalarMultiply(const Matrix<4, 1> &vector) const
    requires(Rows == 4 && Cols == 1)
{
    double result = 0;

    for (int i = 0; i < Rows; ++i)
        result += values[i][0] * vector.values[i][0];

    return result;
}

template <int Rows, int Cols>
const Matrix<4, 1> Matrix<Rows, Cols>::vectorMultiply(const Matrix<4, 1> &vector) const
    requires(Rows == 4 && Cols == 1)
{
    Matrix<4, 1> temp;

    temp.getValue(0, 0) = (cGetY() * vector.cGetZ()) - (cGetZ() * vector.cGetY());
    temp.getValue(1, 0) = (cGetZ() * vector.cGetX()) - (cGetX() * vector.cGetZ());
    temp.getValue(2, 0) = (cGetX() * vector.cGetY()) - (cGetY() * vector.cGetX());
    temp.getValue(3, 0) = 1;

    return temp;
}

template <int Rows, int Cols>
const double Matrix<Rows, Cols>::getLength()
    requires(Rows == 4 && Cols == 1)
{
    if (length.has_value())
        return *length;

    length = sqrt(
        pow(values[0][0], 2) +
        pow(values[1][0], 2) +
        pow(values[2][0], 2));

    return *length;
}

template <int Rows, int Cols>
void Matrix<Rows, Cols>::normalize()
    requires(Rows == 4 && Cols == 1)
{
    if (getLength() != 0.f)
        *this = *this * (1 / getLength());
    else
        *this = Matrix<Rows, Cols>();
}

template <int Rows, int Cols>
Matrix<4, 4> Matrix<Rows, Cols>::getConvertMatrix(
    const Matrix<4, 1> &xAxis,
    const Matrix<4, 1> &yAxis,
    const Matrix<4, 1> &zAxis,
    const Matrix<4, 1> &translation)
{
    auto multiplier = Matrix<4, 4>();

    multiplier.getValue(0, 0) = xAxis.cGetX();
    multiplier.getValue(0, 1) = yAxis.cGetX();
    multiplier.getValue(0, 2) = zAxis.cGetX();
    multiplier.getValue(0, 3) = translation.cGetX();

    multiplier.getValue(1, 0) = xAxis.cGetY();
    multiplier.getValue(1, 1) = yAxis.cGetY();
    multiplier.getValue(1, 2) = zAxis.cGetY();
    multiplier.getValue(1, 3) = translation.cGetY();

    multiplier.getValue(2, 0) = xAxis.cGetZ();
    multiplier.getValue(2, 1) = yAxis.cGetZ();
    multiplier.getValue(2, 2) = zAxis.cGetZ();
    multiplier.getValue(2, 3) = translation.cGetZ();

    multiplier.getValue(3, 0) = xAxis.cGetW();
    multiplier.getValue(3, 1) = yAxis.cGetW();
    multiplier.getValue(3, 2) = zAxis.cGetW();
    multiplier.getValue(3, 3) = translation.cGetW();

    return multiplier;
}

template <int Rows, int Cols>
Matrix<4, 4> Matrix<Rows, Cols>::getMoveConvert(const Matrix<4, 1> &translation)
{
    return getConvertMatrix(
        {1, 0, 0, 0},
        {0, 1, 0, 0},
        {0, 0, 1, 0},
        translation);
}

template <int Rows, int Cols>
Matrix<4, 4> Matrix<Rows, Cols>::getScaleConvert(const Matrix<4, 1> &scale)
{
    return getConvertMatrix(
        {scale.cGetX(), 0, 0, 0},
        {0, scale.cGetY(), 0, 0},
        {0, 0, scale.cGetZ(), 0},
        {0, 0, 0, 1});
}

template <int Rows, int Cols>
Matrix<4, 4> Matrix<Rows, Cols>::getRotateConvert(const AxisName axis, const double angle)
{
    const auto radAngle = angle * M_PI / 180;

    const auto cosA = cos(radAngle);
    const auto sinA = sin(radAngle);

    switch (axis)
    {
    case AxisName::X:
        return getConvertMatrix(
            {1, 0, 0, 0},
            {0, cosA, sinA, 0},
            {0, -sinA, cosA, 0},
            {0, 0, 0, 1});
        break;
    case AxisName::Y:
        return getConvertMatrix(
            {cosA, 0, -sinA, 0},
            {0, 1, 0, 0},
            {sinA, 0, cosA, 0},
            {0, 0, 0, 1});
        break;
    case AxisName::Z:
        return getConvertMatrix(
            {cosA, sinA, 0, 0},
            {-sinA, cosA, 0, 0},
            {0, 0, 1, 0},
            {0, 0, 0, 1});
        break;
    }
}

template <int Rows, int Cols>
Matrix<4, 4> Matrix<Rows, Cols>::getObserverConvert(const Matrix<4, 1> &eye, const Matrix<4, 1> &target, const Matrix<4, 1> &up)
{
    Matrix<4, 1> zAxis = eye - target;
    Matrix<4, 1> xAxis = up.vectorMultiply(zAxis);
    Matrix<4, 1> yAxis = zAxis.vectorMultiply(xAxis);

    xAxis.normalize();
    yAxis.normalize();
    zAxis.normalize();

    return getConvertMatrix(
        {xAxis.cGetX(), yAxis.cGetX(), zAxis.cGetX(), 0},
        {xAxis.cGetY(), yAxis.cGetY(), zAxis.cGetY(), 0},
        {xAxis.cGetZ(), yAxis.cGetZ(), zAxis.cGetZ(), 0},
        {-xAxis.scalarMultiply(eye), -yAxis.scalarMultiply(eye), -zAxis.scalarMultiply(eye), 1});
}

template <int Rows, int Cols>
Matrix<4, 4> Matrix<Rows, Cols>::getProjectionConvert(const double fov, const double aspect, const double zFar, const double zNear)
{
    auto halfTanFov = tan(fov / 2);

    return getConvertMatrix(
        {1.0 / (aspect * halfTanFov), 0, 0, 0},
        {0, 1.0 / halfTanFov, 0, 0},
        {0, 0, zFar / (zNear - zFar), -1},
        {0, 0, (zNear * zFar) / (zNear - zFar), 0});
}

template <int Rows, int Cols>
Matrix<4, 4> Matrix<Rows, Cols>::getWindowConvert(const double width, const double height, const double xMin, const double yMin)
{
    return getConvertMatrix(
        {width / 2, 0, 0, 0},
        {0, -height / 2, 0, 0},
        {0, 0, 1, 0},
        {xMin + width / 2, yMin + height / 2, 0, 1});
}

template <int Rows, int Cols>
void Matrix<Rows, Cols>::log()
{
    for (auto i = 0; i < Rows; ++i)
    {
        for (auto j = 0; j < Cols; ++j)
            std::cout << values[i][j] << ' ';
        std::cout << std::endl;
    }

    std::cout << std::endl;
}

#pragma endregion FUNCTIONS

template class Matrix<4, 4>;
template class Matrix<4, 1>;
template Matrix<4, 4> Matrix<4, 4>::operator*(const Matrix<4, 4> &) const;
template Matrix<4, 1> Matrix<4, 4>::operator*(const Matrix<4, 1> &) const;