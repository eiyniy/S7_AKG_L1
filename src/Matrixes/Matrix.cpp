#include <iostream>
#include <Matrix.hpp>
#include <CoordinateVector.hpp>
#include <MatrixDynamicStorage.hpp>
#include <MatrixStaticStorage.hpp>

#pragma region CONSTRUCTION

Matrix::Matrix(MatrixBaseStorage *p_storage, bool p_isCoordinateVector)
    : storage(p_storage), isCoordinateVector(p_isCoordinateVector) {}

Matrix::~Matrix()
{
    if (!isCoordinateVector)
        delete storage;
}

// TODO: FIX STRORAGE ALLOCATION
Matrix::Matrix(const Matrix &m)
{
    isCoordinateVector = m.isCoordinateVector;
    storage = new MatrixDynamicStorage(m.getRows(), m.getCols());

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

    if (storage == nullptr || getRows() != m.getRows() || getCols() != m.getCols())
        storage = new MatrixDynamicStorage(m.getRows(), m.getCols());

    for (int i = 0; i < getRows(); ++i)
    {
        for (int j = 0; j < getCols(); ++j)
            storage->get(i, j) = m.storage->get(i, j);
    }

    return *this;
}

Matrix::operator CoordinateVector() const
{
    return CoordinateVector(storage->get(0, 0), storage->get(1, 0), storage->get(2, 0), storage->get(3, 0));
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

double &Matrix::getValue(const int i, const int j) const
{
    return storage->get(i, j);
}

Matrix Matrix::getConvertMatrix(const CoordinateVector &xAxis, const CoordinateVector &yAxis, const CoordinateVector &zAxis, const CoordinateVector &translation)
{
    auto multiplier = Matrix(MatrixStaticStorage<4, 4>::getNewPooled());

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

Matrix Matrix::getMoveConvert(const CoordinateVector &translation)
{
    return getConvertMatrix(
        {1, 0, 0, 0},
        {0, 1, 0, 0},
        {0, 0, 1, 0},
        translation);
}

Matrix Matrix::getScaleConvert(const CoordinateVector &scale)
{
    return getConvertMatrix(
        {scale.cGetX(), 0, 0, 0},
        {0, scale.cGetY(), 0, 0},
        {0, 0, scale.cGetZ(), 0},
        {0, 0, 0, 1});
}

Matrix Matrix::getRotateConvert(const AxisName axis, const double angle)
{
    switch (axis)
    {
    case AxisName::X:
        return getConvertMatrix(
            {1, 0, 0, 0},
            {0, cos(angle), sin(angle), 0},
            {0, -sin(angle), cos(angle), 0},
            {0, 0, 0, 1});
        break;
    case AxisName::Y:
        return getConvertMatrix(
            {cos(angle), 0, -sin(angle), 0},
            {0, 1, 0, 0},
            {sin(angle), 0, cos(angle), 0},
            {0, 0, 0, 1});
        break;
    case AxisName::Z:
        return getConvertMatrix(
            {cos(angle), 0, -sin(angle), 0},
            {0, 1, 0, 0},
            {sin(angle), 0, cos(angle), 0},
            {0, 0, 0, 1});
        break;
    }
}

Matrix Matrix::getObserverConvert(const CoordinateVector &eye, const CoordinateVector &target, const CoordinateVector &up)
{
    CoordinateVector zAxis = eye - target;
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

Matrix Matrix::getProjectionConvert(const double fov, const double aspect, const double zFar, const double zNear)
{
    return getConvertMatrix(
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
            (zNear * zFar) / (zNear - zFar),
            0,
        });
}

Matrix Matrix::getWindowConvert(const double width, const double height, const double xMin, const double yMin)
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

void Matrix::log()
{
    for (auto i = 0; i < storage->rows; ++i)
    {
        for (auto j = 0; j < storage->cols; ++j)
            std::cout << storage->get(i, j) << ' ';
        std::cout << std::endl;
    }

    std::cout << std::endl;
}

#pragma endregion FUNCTIONS

#pragma region DUAL_OPERATORS

Matrix operator+(const Matrix &m1, const Matrix &m2)
{
    if (m1.getRows() != m2.getRows() || m1.getCols() != m2.getCols())
        throw std::logic_error("Could not execute + operator");

    auto newStorage = new MatrixDynamicStorage(m1.getRows(), m2.getCols());
    auto temp = Matrix(newStorage);

    for (int i = 0; i < m1.getRows(); ++i)
    {
        for (int j = 0; j < m1.getCols(); ++j)
            temp.getValue(i, j) = m1.getValue(i, j) + m2.getValue(i, j);
    }

    return temp;
}

Matrix operator-(const Matrix &m1, const Matrix &m2)
{
    if (m1.getRows() != m2.getRows() || m1.getCols() != m2.getCols())
        throw std::logic_error("Could not execute - operator");

    auto newStorage = new MatrixDynamicStorage(m1.getRows(), m2.getCols());
    auto temp = Matrix(newStorage);

    for (int i = 0; i < m1.getRows(); ++i)
    {
        for (int j = 0; j < m1.getCols(); ++j)
            temp.getValue(i, j) = m1.getValue(i, j) - m2.getValue(i, j);
    }

    return temp;
}

Matrix operator*(const Matrix &m1, const Matrix &m2)
{
    if (m1.getCols() != m2.getRows())
        throw std::logic_error("Could not execute vector multiply");

    auto newStorage = new MatrixDynamicStorage(m1.getRows(), m2.getCols());
    auto temp = Matrix(newStorage);

    for (int i = 0; i < temp.getRows(); ++i)
    {
        for (int k = 0; k < m1.getCols(); ++k)
        {
            for (int j = 0; j < temp.getCols(); ++j)
                temp.getValue(i, j) += (m1.getValue(i, k) * m2.getValue(k, j));
        }
    }

    return temp;
}

Matrix operator*(const Matrix &m, double v)
{
    auto newStorage = new MatrixDynamicStorage(m.getRows(), m.getCols());
    auto temp = Matrix(newStorage);

    for (int i = 0; i < m.getRows(); ++i)
    {
        for (int j = 0; j < m.getCols(); ++j)
            temp.getValue(i, j) = m.getValue(i, j) * v;
    }

    return temp;
}

Matrix operator*(double v, const Matrix &m)
{
    return (m * v);
}

Matrix operator/(const Matrix &m, double v)
{
    auto newStorage = new MatrixDynamicStorage(m.getRows(), m.getCols());
    auto temp = Matrix(newStorage);

    for (int i = 0; i < m.getRows(); ++i)
    {
        for (int j = 0; j < m.getCols(); ++j)
            temp.getValue(i, j) = m.getValue(i, j) / v;
    }

    return temp;
}

#pragma endregion DUAL_OPERATORS
