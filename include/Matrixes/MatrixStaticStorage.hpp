#pragma once

#include <MatrixBaseStorage.hpp>
#include <Pool.hpp>
#include <array>
#include <stdexcept>

template <int Rows, int Cols>
class MatrixStaticStorage : public MatrixBaseStorage
{
public:
    MatrixStaticStorage();

    static MatrixStaticStorage *getNewPooled();

    void operator delete(void *pt);

    double &get(const int row, const int col);

private:
    std::array<std::array<double, Cols>, Rows> values;
    // double values[Rows][Cols];
};

template <int Rows, int Cols>
inline MatrixStaticStorage<Rows, Cols>::MatrixStaticStorage()
    : MatrixBaseStorage(Rows, Cols) {}

template <int Rows, int Cols>
inline MatrixStaticStorage<Rows, Cols> *MatrixStaticStorage<Rows, Cols>::getNewPooled()
{
    return Pool<MatrixStaticStorage<Rows, Cols>>::getInstance().get();
}

template <int Rows, int Cols>
inline void MatrixStaticStorage<Rows, Cols>::operator delete(void *pt)
{
    MatrixStaticStorage<Rows, Cols> *matrixStoragePt = (MatrixStaticStorage<Rows, Cols> *)pt;
    for (std::array<double, Cols> &cols : matrixStoragePt->values)
    {
        for (double &v : cols)
            v = 0;
    }

    Pool<MatrixStaticStorage<Rows, Cols>>::getInstance().put(matrixStoragePt);
}

template <int Rows, int Cols>
inline double &MatrixStaticStorage<Rows, Cols>::get(const int row, const int col)
{
    if (row >= Rows || col >= Cols)
        throw std::out_of_range("Could not get value");

    return values[row][col];
}
