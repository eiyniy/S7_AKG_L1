#pragma once

#include <vector>
#include <stdexcept>

template <class T>
class Matrix
{
public:
    Matrix(int p_rows, int p_cols);
    Matrix(const Matrix &m);
    Matrix &operator=(const Matrix &m);

    Matrix &operator+=(const Matrix &m);
    Matrix &operator-=(const Matrix &m);
    Matrix &operator*=(const Matrix &m);
    Matrix &operator*=(const double v);
    Matrix &operator/=(const double v);

private:
    int rows, cols;
    std::vector<std::vector<T>> values;
};

template <class T>
Matrix<T> operator+(const Matrix<T> &m1, const Matrix<T> &m2);
template <class T>
Matrix<T> operator-(const Matrix<T> &m1, const Matrix<T> &m2);
template <class T>
Matrix<T> operator*(const Matrix<T> &m1, const Matrix<T> &m2);
template <class T>
Matrix<T> operator*(const Matrix<T> &m, double v);
template <class T>
Matrix<T> operator*(double v, const Matrix<T> &m);
template <class T>
Matrix<T> operator/(const Matrix<T> &m, double v);

template <class T>
inline Matrix<T>::Matrix(int p_rows, int p_cols)
    : rows(p_rows), cols(p_cols)
{
    values = std::vector<std::vector<T>>(rows);

    for (int i = 0; i < rows; ++i)
        values[i] = std::vector<T>(cols);
}

template <class T>
Matrix<T>::Matrix(const Matrix &m) : rows(m.rows), cols(m.cols)
{
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
            values[i][j] = m.values[i][j];
    }
}

template <class T>
inline Matrix<T> &Matrix<T>::operator=(const Matrix &m)
{
    if (this == &m)
        return *this;

    if (rows != m.rows || cols != m.cols)
        *this = Matrix<T>(m.rows, m.cols);

    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
            values[i][j] = m.values[i][j];
    }

    return *this;
}

template <class T>
inline Matrix<T> &Matrix<T>::operator+=(const Matrix &m)
{
    if (rows != m.rows || cols != m.cols)
        throw std::invalid_argument("Could not execute += operator");

    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
            values[i][j] += m.values[i][j];
    }

    return *this;
}

template <class T>
inline Matrix<T> &Matrix<T>::operator-=(const Matrix &m)
{
    if (rows != m.rows || cols != m.cols)
        throw std::invalid_argument("Could not execute -= operator");

    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
            values[i][j] -= m.values[i][j];
    }

    return *this;
}

template <class T>
inline Matrix<T> &Matrix<T>::operator*=(const Matrix &m)
{
    auto temp = Matrix<T>(rows, m.cols);

    for (int i = 0; i < temp.rows; ++i)
    {
        for (int j = 0; j < temp.cols; ++j)
        {
            for (int k = 0; k < cols; ++k)
                temp.values[i][j] += (values[i][k] * m.values[k][j]);
        }
    }

    return (*this = temp);
}

template <class T>
inline Matrix<T> &Matrix<T>::operator*=(const double v)
{
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
            values[i][j] *= v;
    }

    return *this;
}

template <class T>
inline Matrix<T> &Matrix<T>::operator/=(const double v)
{
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
            values[i][j] /= v;
    }

    return *this;
}

template <class T>
inline Matrix<T> operator+(const Matrix<T> &m1, const Matrix<T> &m2)
{
    Matrix temp(m1);
    return (temp += m2);
}

template <class T>
inline Matrix<T> operator-(const Matrix<T> &m1, const Matrix<T> &m2)
{
    Matrix temp(m1);
    return (temp -= m2);
}

template <class T>
inline Matrix<T> operator*(const Matrix<T> &m1, const Matrix<T> &m2)
{
    Matrix temp(m1);
    return (temp *= m2);
}

template <class T>
inline Matrix<T> operator*(const Matrix<T> &m, double v)
{
    Matrix temp(m);
    return (temp *= v);
}

template <class T>
inline Matrix<T> operator*(double v, const Matrix<T> &m)
{
    return (m * v);
}

template <class T>
inline Matrix<T> operator/(const Matrix<T> &m, double v)
{
    Matrix temp(m);
    return (temp /= v);
}
