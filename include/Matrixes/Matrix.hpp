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
    Matrix(MatrixBaseStorage *p_storage, bool p_isCoordinateVector = false);
    ~Matrix();

    Matrix(const Matrix &m);
    Matrix &operator=(const Matrix &m);

    operator CoordinateVector() const;

    const double getCols() const;
    const double getRows() const;

    double &getValue(const int i, const int j) const;

    static Matrix getConvertMatrix(
        const CoordinateVector &xAxis,
        const CoordinateVector &yAxis,
        const CoordinateVector &zAxis,
        const CoordinateVector &translation);

    static Matrix getMoveConvert(
        const CoordinateVector &translation);

    static Matrix getScaleConvert(
        const CoordinateVector &scale);

    static Matrix getRotateConvert(
        const AxisName axis,
        const double angle);

    static Matrix getObserverConvert(
        const CoordinateVector &eye,
        const CoordinateVector &target,
        const CoordinateVector &up);

    static Matrix getProjectionConvert(
        const double fov,
        const double aspect,
        const double zFar,
        const double zNear);

    static Matrix getWindowConvert(
        const double width,
        const double height,
        const double xMin,
        const double yMin);

    void log();

protected:
    std::optional<double> length;
    MatrixBaseStorage *storage;

private:
    bool isCoordinateVector;
};

Matrix operator+(const Matrix &m1, const Matrix &m2);
Matrix operator-(const Matrix &m1, const Matrix &m2);
Matrix operator*(const Matrix &m1, const Matrix &m2);
Matrix operator*(const Matrix &m, double v);
Matrix operator*(double v, const Matrix &m);
Matrix operator/(const Matrix &m, double v);
