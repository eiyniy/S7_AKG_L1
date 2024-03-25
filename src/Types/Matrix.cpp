#include <iostream>
#include <cmath>
#include <format>
#include <Matrix.hpp>

#pragma region CONSTRUCTION

template <int Rows, int Cols>
Matrix<Rows, Cols>::Matrix()
{
    for (int i = 0; i < Rows; ++i)
    {
        for (int j = 0; j < Cols; ++j)
            getValue(i, j) = 0;
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
    getX() = x;
    getY() = y;
    getZ() = z;
    getW() = w;
}

template <int Rows, int Cols>
Matrix<Rows, Cols>::Matrix(const Matrix &m)
{
    for (int i = 0; i < Rows; ++i)
    {
        for (int j = 0; j < Cols; ++j)
            getValue(i, j) = m.cGetValue(i, j);
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
            getValue(i, j) = m.cGetValue(i, j);
    }

    return *this;
}

template <int Rows, int Cols>
Matrix<Rows, Cols> &Matrix<Rows, Cols>::operator+=(const Matrix &m)
{
    for (int i = 0; i < Rows; ++i)
    {
        for (int j = 0; j < Cols; ++j)
            getValue(i, j) += m.cGetValue(i, j);
    }

    return *this;
}

template <int Rows, int Cols>
Matrix<Rows, Cols> &Matrix<Rows, Cols>::operator-=(const Matrix &m)
{
    for (int i = 0; i < Rows; ++i)
    {
        for (int j = 0; j < Cols; ++j)
            getValue(i, j) -= m.cGetValue(i, j);
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
            getValue(i, j) += v;
    }

    return *this;
}

template <int Rows, int Cols>
Matrix<Rows, Cols> &Matrix<Rows, Cols>::operator-=(const double v)
{
    for (int i = 0; i < Rows; ++i)
    {
        for (int j = 0; j < Cols; ++j)
            getValue(i, j) -= v;
    }

    return *this;
}

template <int Rows, int Cols>
Matrix<Rows, Cols> &Matrix<Rows, Cols>::operator*=(const double v)
{
    for (int i = 0; i < Rows; ++i)
    {
        for (int j = 0; j < Cols; ++j)
            getValue(i, j) *= v;
    }

    return *this;
}

template <int Rows, int Cols>
Matrix<Rows, Cols> &Matrix<Rows, Cols>::operator/=(const double v)
{
    for (int i = 0; i < Rows; ++i)
    {
        for (int j = 0; j < Cols; ++j)
            getValue(i, j) /= v;
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
                temp.getValue(i, j) += cGetValue(i, k) * m.cGetValue(k, j);
        }
    }

    return temp;
}

#pragma endregion OPERATORS

#pragma region FUNCTIONS

template <int Rows, int Cols>
double Matrix<Rows, Cols>::scalarMultiply(const Vector<4> &vector) const
    requires(Rows == 4 && Cols == 1)
{
    double result = 0;

    for (int i = 0; i < Rows; ++i)
        result += values[i] * vector.values[i];

    return result;
}

template <int Rows, int Cols>
Vector<4> Matrix<Rows, Cols>::vectorMultiply(const Vector<4> &vector) const
    requires(Rows == 4 && Cols == 1)
{
    Vector<4> temp;

    // temp.values[0] = cGetY() * vector.cGetZ() - cGetZ() * vector.cGetY();
    // temp.values[1] = cGetZ() * vector.cGetX() - cGetX() * vector.cGetZ();
    // temp.values[2] = cGetX() * vector.cGetY() - cGetY() * vector.cGetX();
    // temp.values[3] = 1;

    temp.getValue(0, 0) = (cGetY() * vector.cGetZ()) - (cGetZ() * vector.cGetY());
    temp.getValue(1, 0) = (cGetZ() * vector.cGetX()) - (cGetX() * vector.cGetZ());
    temp.getValue(2, 0) = (cGetX() * vector.cGetY()) - (cGetY() * vector.cGetX());
    //    temp.getValue(3, 0) = 1;
    temp.getValue(3, 0) = 0;

    return temp;
}

template <int Rows, int Cols>
double Matrix<Rows, Cols>::getLength() const
    requires(Rows == 4 && Cols == 1)
{
    if (length.has_value())
        return *length;

    length = sqrt(
        pow(values[0], 2) +
        pow(values[1], 2) +
        pow(values[2], 2));

    return *length;
}

template <int Rows, int Cols>
void Matrix<Rows, Cols>::normalize()
    requires(Rows == 4 && Cols == 1)
{
    if (getLength() != 0.f)
        *this /= getLength();
    else
        *this = Matrix<Rows, Cols>();
}

template <int Rows, int Cols>
Matrix<4, 4> Matrix<Rows, Cols>::getConvertMatrix(
    const Vector<4> &xAxis,
    const Vector<4> &yAxis,
    const Vector<4> &zAxis,
    const Vector<4> &translation)
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
Matrix<4, 4> Matrix<Rows, Cols>::getMoveConvert(const Vector<4> &translation)
{
    return getConvertMatrix(
        {1, 0, 0, 0},
        {0, 1, 0, 0},
        {0, 0, 1, 0},
        {translation.cGetX(), translation.cGetY(), translation.cGetZ(), 1});
}

template <int Rows, int Cols>
Matrix<4, 4> Matrix<Rows, Cols>::getScaleConvert(const Vector<4> &scale)
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
    case AxisName::Y:
        return getConvertMatrix(
            {cosA, 0, -sinA, 0},
            {0, 1, 0, 0},
            {sinA, 0, cosA, 0},
            {0, 0, 0, 1});
    case AxisName::Z:
        return getConvertMatrix(
            {cosA, sinA, 0, 0},
            {-sinA, cosA, 0, 0},
            {0, 0, 1, 0},
            {0, 0, 0, 1});
    }
}

template <int Rows, int Cols>
Matrix<4, 4>
Matrix<Rows, Cols>::getViewConvert(const Vector<4> &eye, const Vector<4> &target, const Vector<4> &up)
{
    Vector<4> zAxis = eye - target;
    Vector<4> xAxis = up.vectorMultiply(zAxis);
    Vector<4> yAxis = zAxis.vectorMultiply(xAxis);

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
Matrix<4, 4>
Matrix<Rows, Cols>::getProjectionConvert(const double fov, const double aspect, const double zFar, const double zNear)
{
    const auto halfTanFov = tan(fov / 2);

    return getConvertMatrix(
        {1.0 / (aspect * halfTanFov), 0, 0, 0},
        {0, 1.0 / halfTanFov, 0, 0},
        {0, 0, zFar / (zNear - zFar), -1},
        {0, 0, (zNear * zFar) / (zNear - zFar), 0});
}

template <int Rows, int Cols>
Matrix<4, 4>
Matrix<Rows, Cols>::getViewportConvert(const double width, const double height, const double xMin, const double yMin)
{
    return getConvertMatrix(
        {width / 2, 0, 0, 0},
        {0, -height / 2, 0, 0},
        {0, 0, 1, 0},
        {xMin + width / 2, yMin + height / 2, 0, 1});
}

template <int Rows, int Cols>
void Matrix<Rows, Cols>::log() const
{
    for (auto i = 0; i < Rows; ++i)
    {
        for (auto j = 0; j < Cols; ++j)
            std::cout << cGetValue(i, j) << ' ';
        std::cout << std::endl;
    }

    std::cout << std::endl;
}

template <>
void Vector<4>::log() const
{
    for (auto i = 0; i < 4; ++i)
        std::cout << std::format("{:5.3f}", values[i]) << ' ';

    std::cout << std::endl;
}

#pragma endregion FUNCTIONS

template class Matrix<4, 4>;

template class Matrix<4, 1>;

template Matrix<4, 4> Matrix<4, 4>::operator*(const Matrix<4, 4> &) const;

template Vector<4> Matrix<4, 4>::operator*(const Vector<4> &) const;