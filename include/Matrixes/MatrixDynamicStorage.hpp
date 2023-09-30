#pragma once

#include <vector>
#include <MatrixBaseStorage.hpp>

class MatrixDynamicStorage : public MatrixBaseStorage
{
public:
    MatrixDynamicStorage(const int rows, const int cols);

    double &get(const int row, const int col);

private:
    std::vector<std::vector<double>> values;
};
