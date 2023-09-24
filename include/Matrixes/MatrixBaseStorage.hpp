#pragma once

class MatrixBaseStorage
{
public:
    MatrixBaseStorage(const int p_rows, const int p_cols);

    virtual double &get(const int row, const int col) = 0;
    virtual void set(const int row, const int col, const double value) = 0;

    const int cols, rows;
};
