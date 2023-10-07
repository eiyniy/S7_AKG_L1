#include <CoordinateVector.hpp>
#include <Timer.hpp>
#include <Vertex.hpp>
#include <Pool.hpp>
#include <Converter.hpp>
#include <iostream>

CoordinateVector::CoordinateVector() : Matrix<4, 1>(){};

CoordinateVector::CoordinateVector(const CoordinateVector &vector)
    : Matrix<4, 1>()
{
    for (int i = 0; i < rows; ++i)
        values[i][0] = vector.values[i][0];
}

CoordinateVector::CoordinateVector(double v1, double v2, double v3, double w)
    : Matrix<4, 1>()
{
    values[0][0] = v1;
    values[1][0] = v2;
    values[2][0] = v3;
    values[3][0] = w;
}

CoordinateVector &CoordinateVector::operator=(const CoordinateVector &cv)
{
    if (this == &cv)
        return *this;

    for (int i = 0; i < rows; ++i)
        values[i][0] = cv.values[i][0];

    return *this;
}

const double CoordinateVector::cGetX() const
{
    return values[0][0];
}

const double CoordinateVector::cGetY() const
{
    return values[1][0];
}

const double CoordinateVector::cGetZ() const
{
    return values[2][0];
}

const double CoordinateVector::cGetW() const
{
    return values[3][0];
}

double &CoordinateVector::getX()
{
    return values[0][0];
}

double &CoordinateVector::getY()
{
    return values[1][0];
}

double &CoordinateVector::getZ()
{
    return values[2][0];
}

double &CoordinateVector::getW()
{
    return values[3][0];
}

double CoordinateVector::scalarMultiply(const CoordinateVector &vector)
{
    if (this->cols != vector.cols)
        throw std::logic_error("Can't execute scalar multiply");

    double result = 0;

    for (int i = 0; i < this->rows; ++i)
        result += values[i][0] * vector.values[i][0];

    return result;
}

const double CoordinateVector::getLength()
{
    if (length.has_value())
        return *length;

    length = sqrt(
        pow(values[0][0], 2) +
        pow(values[1][0], 2) +
        pow(values[2][0], 2));

    return *length;
}

CoordinateVector CoordinateVector::getNormalized()
{
    if (getLength() != 0.f)
    {
        auto matrix = *this * (1 / getLength());
        return Converter::matrixToCVector(matrix);
    }
    else
        return CoordinateVector();
}

void CoordinateVector::log() const
{
    std::cout << cGetX() << " " << cGetY() << " " << cGetZ() << " " << cGetW() << " " << std::endl;
}

CoordinateVector operator+(const CoordinateVector &cv1, const CoordinateVector &cv2)
{
    CoordinateVector temp = cv1;

    temp.getX() += cv2.cGetX();
    temp.getY() += cv2.cGetY();
    temp.getZ() += cv2.cGetZ();
    temp.getW() += cv2.cGetW();

    return temp;
}

CoordinateVector operator*(const CoordinateVector &cv1, const CoordinateVector &cv2)
{
    CoordinateVector temp;

    temp.getValue(0, 0) = (cv1.cGetY() * cv2.cGetZ()) - (cv1.cGetZ() * cv2.cGetY());
    temp.getValue(1, 0) = (cv1.cGetZ() * cv2.cGetX()) - (cv1.cGetX() * cv2.cGetZ());
    temp.getValue(2, 0) = (cv1.cGetX() * cv2.cGetY()) - (cv1.cGetY() * cv2.cGetX());
    temp.getValue(3, 0) = 1;

    return temp;
}

CoordinateVector operator/(const CoordinateVector &cv1, const double v)
{
    CoordinateVector temp = cv1;

    temp.getX() /= v;
    temp.getY() /= v;
    temp.getZ() /= v;
    temp.getW() /= v;

    return temp;
}
