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

    static Matrix fromCoordinats(double v1, double v2, double v3);

    Matrix &operator+=(const Matrix &m);
    Matrix &operator-=(const Matrix &m);
    Matrix &operator*=(const Matrix &m);
    Matrix &operator*=(const double v);
    Matrix &operator/=(const double v);

    const double getCols() const;
    const double getRows() const;

    const double getValue(const int i, const int j) const;

    void convert(
        CoordinateVector xAxis,
        CoordinateVector yAxis,
        CoordinateVector zAxis,
        CoordinateVector translation);

    void moveConvert(
        CoordinateVector &translation);

    void scaleConvert(
        CoordinateVector &scale);

    void rotateConvert(
        AxisName axis,
        double angle);

    void toViewerConvert(
        CoordinateVector &eye,
        CoordinateVector &target,
        CoordinateVector &up);

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
