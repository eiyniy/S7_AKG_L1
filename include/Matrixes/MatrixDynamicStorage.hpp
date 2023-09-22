#pragma once

#include <vector>
#include <MatrixStorageProvider.hpp>

class MatrixDynamicStorage : public MatrixStorageProvider
{
public:
    MatrixDynamicStorage(int rows, int cols);

    double &get(const int row, const int col);
    void set(const int row, const int col, const double value);

private:
    std::vector<std::vector<double>> values;
};
