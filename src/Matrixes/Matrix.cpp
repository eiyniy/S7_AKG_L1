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
        throw std::logic_error("Could not execute += operator");

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
        throw std::logic_error("Could not execute -= operator");

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
