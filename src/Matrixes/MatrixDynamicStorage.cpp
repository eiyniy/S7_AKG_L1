#include <MatrixDynamicStorage.hpp>

MatrixDynamicStorage::MatrixDynamicStorage(int rows, int cols)
    : MatrixBaseStorage(rows, cols)
{
    values = std::vector<std::vector<double>>(rows);

    for (int i = 0; i < rows; ++i)
        values[i] = std::vector<double>(cols);
}

double &MatrixDynamicStorage::get(const int row, const int col)
{
    return values[row][col];
}
