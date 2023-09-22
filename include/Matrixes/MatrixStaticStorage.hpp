#pragma once

#include <MatrixStorageProvider.hpp>
#include <array>
#include <stdexcept>

template <int Rows, int Cols>
class MatrixStaticStorage : public MatrixStorageProvider
{
public:
    MatrixStaticStorage();

    double &get(const int row, const int col);
    void set(const int row, const int col, const double value);

private:
    std::array<std::array<double, Cols>, Rows> values;
};

template <int Rows, int Cols>
inline MatrixStaticStorage<Rows, Cols>::MatrixStaticStorage()
    : MatrixStorageProvider(Rows, Cols) {}

template <int Rows, int Cols>
inline double &MatrixStaticStorage<Rows, Cols>::get(const int row, const int col)
{
    if (row >= Rows || col >= Cols)
        throw std::out_of_range("Could not get value");

    return values[row][col];
}

template <int Rows, int Cols>
inline void MatrixStaticStorage<Rows, Cols>::set(const int row, const int col, const double value)
{
    if (row >= Rows || col >= Cols)
        throw std::out_of_range("Could not get value");

    values[row][col] = value;
}
