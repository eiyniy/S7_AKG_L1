#include <Matrix.hpp>
#include <CoordinateVector.hpp>
#include <MatrixDynamicStorage.hpp>
#include <MatrixStaticStorage.hpp>

#pragma region CONSTRUCTION

Matrix::Matrix(MatrixBaseStorage *p_storage)
    : storage(p_storage) {}

Matrix::~Matrix()
{
    delete storage;
}

// TODO: FIX THIS - ADD STRORAGE ALLOCATION
Matrix::Matrix(const Matrix &m)
{
    for (int i = 0; i < getRows(); ++i)
    {
        for (int j = 0; j < getCols(); ++j)
            storage->get(i, j) = m.storage->get(i, j);
    }
}

#pragma endregion CONSTRUCTION

#pragma region OPERATORS

Matrix &Matrix::operator=(const Matrix &m)
{
    if (this == &m)
        return *this;

    if (getRows() != m.getRows() || getCols() != m.getCols())
    {
        auto storage = new MatrixDynamicStorage(m.getRows(), m.getCols());
        *this = Matrix(storage);
    }

    for (int i = 0; i < getRows(); ++i)
    {
        for (int j = 0; j < getCols(); ++j)
            storage->get(i, j) = m.storage->get(i, j);
    }

    return *this;
}

Matrix Matrix::fromCoordinats(double v1, double v2, double v3)
{
    auto m = Matrix(MatrixStaticStorage<1, 3>::getNewPooled());

    m.storage->get(0, 0) = v1;
    m.storage->get(0, 1) = v2;
    m.storage->get(0, 2) = v3;

    return m;
}

Matrix &Matrix::operator+=(const Matrix &m)
{
    if (getRows() != m.getRows() || getCols() != m.getCols())
        throw std::logic_error("Could not execute += operator");

    for (int i = 0; i < getRows(); ++i)
    {
        for (int j = 0; j < getCols(); ++j)
            storage->get(i, j) += m.storage->get(i, j);
    }

    return *this;
}

Matrix &Matrix::operator-=(const Matrix &m)
{
    if (getRows() != m.getRows() || getCols() != m.getCols())
        throw std::logic_error("Could not execute -= operator");

    for (int i = 0; i < getRows(); ++i)
    {
        for (int j = 0; j < getCols(); ++j)
            storage->get(i, j) -= m.storage->get(i, j);
    }

    return *this;
}

Matrix &Matrix::operator*=(const Matrix &m)
{
    if (getCols() != m.getRows())
        throw std::logic_error("Could not execute vector multiply");

    auto storage = new MatrixDynamicStorage(getRows(), m.getCols());
    auto temp = Matrix(storage);

    for (int i = 0; i < temp.getRows(); ++i)
    {
        for (int j = 0; j < temp.getCols(); ++j)
        {
            for (int k = 0; k < getCols(); ++k)
                temp.storage->get(i, j) += (storage->get(i, k) * m.storage->get(k, j));
        }
    }

    return (*this = temp);
}

Matrix &Matrix::operator*=(const double v)
{
    for (int i = 0; i < getRows(); ++i)
    {
        for (int j = 0; j < getCols(); ++j)
            storage->get(i, j) *= v;
    }

    return *this;
}

Matrix &Matrix::operator/=(const double v)
{
    for (int i = 0; i < getRows(); ++i)
    {
        for (int j = 0; j < getCols(); ++j)
            storage->get(i, j) /= v;
    }

    return *this;
}

#pragma endregion OPERATORS

#pragma region FUNCTIONS

const double Matrix::getCols() const
{
    return storage->cols;
}

const double Matrix::getRows() const
{
    return storage->rows;
}

const double Matrix::getValue(const int i, const int j) const
{
    return storage->get(i, j);
}

#pragma endregion FUNCTIONS

#pragma region CONVERTIONS

void Matrix::convert(
    CoordinateVector xAxis,
    CoordinateVector yAxis,
    CoordinateVector zAxis,
    CoordinateVector translation)
{
    Matrix multiplier{MatrixStaticStorage<4, 4>::getNewPooled()};

    multiplier.storage->get(0, 0) = xAxis.getX();
    multiplier.storage->get(0, 1) = yAxis.getX();
    multiplier.storage->get(0, 2) = zAxis.getX();
    multiplier.storage->get(0, 3) = translation.getX();

    multiplier.storage->get(1, 0) = xAxis.getY();
    multiplier.storage->get(1, 1) = yAxis.getY();
    multiplier.storage->get(1, 2) = zAxis.getY();
    multiplier.storage->get(1, 3) = translation.getY();

    multiplier.storage->get(2, 0) = xAxis.getZ();
    multiplier.storage->get(2, 1) = yAxis.getZ();
    multiplier.storage->get(2, 2) = zAxis.getZ();
    multiplier.storage->get(2, 3) = translation.getZ();

    multiplier.storage->get(3, 0) = xAxis.getW();
    multiplier.storage->get(3, 1) = yAxis.getW();
    multiplier.storage->get(3, 2) = zAxis.getW();
    multiplier.storage->get(3, 3) = translation.getW();

    *this *= multiplier;
}

void Matrix::moveConvert(CoordinateVector &translation)
{
    convert(
        {1, 0, 0, 0},
        {0, 1, 0, 0},
        {0, 0, 1, 0},
        translation);
}

void Matrix::scaleConvert(CoordinateVector &scale)
{
    convert(
        {scale.getX(), 0, 0, 0},
        {0, scale.getY(), 0, 0},
        {0, 0, scale.getZ(), 0},
        {0, 0, 0, 1});
}

void Matrix::rotateConvert(AxisName axis, double angle)
{
    switch (axis)
    {
    case AxisName::X:
        convert(
            {1, 0, 0, 0},
            {0, cos(angle), sin(angle), 0},
            {0, -sin(angle), cos(angle), 0},
            {0, 0, 0, 1});
        break;
    case AxisName::Y:
        convert(
            {cos(angle), 0, -sin(angle), 0},
            {0, 1, 0, 0},
            {sin(angle), 0, cos(angle), 0},
            {0, 0, 0, 1});
        break;
    case AxisName::Z:
        convert(
            {cos(angle), 0, -sin(angle), 0},
            {0, 1, 0, 0},
            {sin(angle), 0, cos(angle), 0},
            {0, 0, 0, 1});
        break;
    }
}

void Matrix::toObserverConvert(
    CoordinateVector &eye,
    CoordinateVector &target,
    CoordinateVector &up)
{
    auto zAxisBase = eye - target;
    auto xAxisBase = up * zAxisBase;

    auto zAxis = static_cast<CoordinateVector *>(&zAxisBase)->getNormalized();
    auto xAxis = static_cast<CoordinateVector *>(&xAxisBase)->getNormalized();
    auto yAxis = up;

    convert(
        {
            xAxis.getX(),
            yAxis.getX(),
            zAxis.getX(),
            0,
        },
        {
            xAxis.getY(),
            yAxis.getY(),
            zAxis.getY(),
            0,
        },
        {
            xAxis.getZ(),
            yAxis.getZ(),
            zAxis.getZ(),
            0,
        },
        {
            -xAxis.scalarMultiply(eye),
            -yAxis.scalarMultiply(eye),
            -zAxis.scalarMultiply(eye),
            1,
        });
}

void Matrix::toProjectionConvert(double fov, double aspect, double zFar, double zNear)
{
    convert(
        {
            1.0 / (aspect * tan(fov / 2)),
            0,
            0,
            0,
        },
        {
            0,
            1.0 / tan(fov / 2),
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
            zNear * zFar / (zNear - zFar),
            0,
        });
}

void Matrix::toViewerConvert(double width, double height, double xMin, double yMin)
{
    convert(
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

#pragma endregion CONVERTIONS

#pragma region DUAL_OPERATORS

Matrix
operator+(const Matrix &m1, const Matrix &m2)
{
    Matrix temp(m1);
    return (temp += m2);
}

Matrix operator-(const Matrix &m1, const Matrix &m2)
{
    Matrix temp(m1);
    return (temp -= m2);
}

Matrix operator*(const Matrix &m1, const Matrix &m2)
{
    Matrix temp(m1);
    return (temp *= m2);
}

Matrix operator*(const Matrix &m, double v)
{
    Matrix temp(m);
    return (temp *= v);
}

Matrix operator*(double v, const Matrix &m)
{
    return (m * v);
}

Matrix operator/(const Matrix &m, double v)
{
    Matrix temp(m);
    return (temp /= v);
}

#pragma endregion DUAL_OPERATORS
