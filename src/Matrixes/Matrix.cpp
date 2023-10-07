#include <iostream>
#include <Matrix.hpp>
#include <CoordinateVector.hpp>
#include <Converter.hpp>

#pragma region CONSTRUCTION

template <int Rows, int Cols>
Matrix<Rows, Cols>::Matrix()
    : rows(Rows), cols(Cols)
{
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
            values[i][j] = 0;
    }
};

template <int Rows, int Cols>
Matrix<Rows, Cols>::~Matrix()
{
}

// TODO: FIX STRORAGE ALLOCATION
template <int Rows, int Cols>
Matrix<Rows, Cols>::Matrix(const Matrix &m)
    : rows(Rows), cols(Cols)
{
    values = std::array<std::array<double, Cols>, Rows>();

    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
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

    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
            values[i][j] = m.values[i][j];
    }

    return *this;
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
Matrix<4, 4> Matrix<Rows, Cols>::getConvertMatrix(
    const CoordinateVector &xAxis,
    const CoordinateVector &yAxis,
    const CoordinateVector &zAxis,
    const CoordinateVector &translation)
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
Matrix<4, 4> Matrix<Rows, Cols>::getMoveConvert(const CoordinateVector &translation)
{
    return getConvertMatrix(
        {1, 0, 0, 0},
        {0, 1, 0, 0},
        {0, 0, 1, 0},
        translation);
}

template <int Rows, int Cols>
Matrix<4, 4> Matrix<Rows, Cols>::getScaleConvert(const CoordinateVector &scale)
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
    auto cosA = cos(angle);
    auto sinA = sin(angle);

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
Matrix<4, 4> Matrix<Rows, Cols>::getObserverConvert(const CoordinateVector &eye, const CoordinateVector &target, const CoordinateVector &up)
{
    CoordinateVector zAxis = Converter::matrixToCVector(eye - target);
    CoordinateVector xAxis = up * zAxis;
    CoordinateVector yAxis = zAxis * xAxis;
    // CoordinateVector yAxis = up;

    xAxis = xAxis.getNormalized();
    yAxis = yAxis.getNormalized();
    zAxis = zAxis.getNormalized();

    return getConvertMatrix(
        {
            xAxis.cGetX(),
            yAxis.cGetX(),
            zAxis.cGetX(),
            0,
        },
        {
            xAxis.cGetY(),
            yAxis.cGetY(),
            zAxis.cGetY(),
            0,
        },
        {
            xAxis.cGetZ(),
            yAxis.cGetZ(),
            zAxis.cGetZ(),
            0,
        },
        {
            -xAxis.scalarMultiply(eye),
            -yAxis.scalarMultiply(eye),
            -zAxis.scalarMultiply(eye),
            1,
        });
}

template <int Rows, int Cols>
Matrix<4, 4> Matrix<Rows, Cols>::getProjectionConvert(const double fov, const double aspect, const double zFar, const double zNear)
{
    auto halfTanFov = tan(fov / 2);

    return getConvertMatrix(
        {
            1.0 / (aspect * halfTanFov),
            0,
            0,
            0,
        },
        {
            0,
            1.0 / halfTanFov,
            0,
            0,
        },
        {
            0,
            0,
            zFar / (zNear - zFar),
            -1,
        },
        {
            0,
            0,
            (zNear * zFar) / (zNear - zFar),
            0,
        });
}

template <int Rows, int Cols>
Matrix<4, 4> Matrix<Rows, Cols>::getWindowConvert(const double width, const double height, const double xMin, const double yMin)
{
    return getConvertMatrix(
        {
            width / 2,
            0,
            0,
            0,
        },
        {
            0,
            -height / 2,
            0,
            0,
        },
        {
            0,
            0,
            1,
            0,
        },
        {
            xMin + width / 2,
            yMin + height / 2,
            0,
            1,
        });
}

template <int Rows, int Cols>
void Matrix<Rows, Cols>::log()
{
    for (auto i = 0; i < rows; ++i)
    {
        for (auto j = 0; j < cols; ++j)
            std::cout << values[i][j] << ' ';
        std::cout << std::endl;
    }

    std::cout << std::endl;
}

#pragma endregion FUNCTIONS

#pragma region DUAL_OPERATORS

template <int Rows, int Cols>
Matrix<Rows, Cols> operator+(const Matrix<Rows, Cols> &m1, const Matrix<Rows, Cols> &m2)
{
    if (m1.rows != m2.rows || m1.cols != m2.cols)
        throw std::logic_error("Could not execute + operator");

    auto temp = Matrix<Rows, Cols>();

    for (int i = 0; i < m1.rows; ++i)
    {
        for (int j = 0; j < m1.cols; ++j)
            temp.getValue(i, j) = m1.getValue(i, j) + m2.getValue(i, j);
    }

    return temp;
}

template <int Rows, int Cols>
Matrix<Rows, Cols> operator-(const Matrix<Rows, Cols> &m1, const Matrix<Rows, Cols> &m2)
{
    if (m1.rows != m2.rows || m1.cols != m2.cols)
        throw std::logic_error("Could not execute - operator");

    auto temp = Matrix<Rows, Cols>();

    for (int i = 0; i < m1.rows; ++i)
    {
        for (int j = 0; j < m1.cols; ++j)
            temp.getValue(i, j) = m1.cGetValue(i, j) - m2.cGetValue(i, j);
    }

    return temp;
}

template <int Rows, int Cols>
Matrix<Rows, Cols> operator*(double v, const Matrix<Rows, Cols> &m)
{
    return (m * v);
}

template <int Rows, int Cols>
Matrix<Rows, Cols> operator/(const Matrix<Rows, Cols> &m, double v)
{
    auto temp = Matrix<Rows, Cols>();

    for (int i = 0; i < m.rows; ++i)
    {
        for (int j = 0; j < m.cols; ++j)
            temp.getValue(i, j) = m.cGetValue(i, j) / v;
    }

    return temp;
}

#pragma endregion DUAL_OPERATORS

template class Matrix<4, 4>;
template class Matrix<4, 1>;