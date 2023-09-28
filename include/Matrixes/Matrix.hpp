#pragma once

#include <MatrixBaseStorage.hpp>
#include <Types.hpp>
#include <vector>
#include <stdexcept>
#include <optional>
#include <cmath>

//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// MATRIX [I] [J]: I->ROW; J->COL

class CoordinateVector;

class Matrix
{
public:
    Matrix(MatrixBaseStorage *p_storage);
    ~Matrix();

    Matrix(const Matrix &m);
    Matrix &operator=(const Matrix &m);

    Matrix &operator+=(const Matrix &m);
    Matrix &operator-=(const Matrix &m);
    Matrix &operator*=(const Matrix &m);
    Matrix &operator*=(const double v);
    Matrix &operator/=(const double v);

    operator CoordinateVector() const;

    const double getCols() const;
    const double getRows() const;

    double &getValue(const int i, const int j) const;

protected:
    std::optional<double> length;
    MatrixBaseStorage *storage;
};

Matrix operator+(const Matrix &m1, const Matrix &m2);
Matrix operator-(const Matrix &m1, const Matrix &m2);
Matrix operator*(const Matrix &m1, const Matrix &m2);
Matrix operator*(const Matrix &m, double v);
Matrix operator*(double v, const Matrix &m);
Matrix operator/(const Matrix &m, double v);
