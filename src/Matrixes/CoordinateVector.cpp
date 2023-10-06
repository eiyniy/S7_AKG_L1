#include <CoordinateVector.hpp>
#include <MatrixStaticStorage.hpp>
#include <Timer.hpp>
#include <Vertex.hpp>
#include <Pool.hpp>
#include <Converter.hpp>
#include <iostream>

CoordinateVector::CoordinateVector()
    : Matrix(MatrixStaticStorage<4, 1>::getNewPooled(), true) {}

CoordinateVector::~CoordinateVector()
{
    delete (MatrixStaticStorage<4, 1> *)storage;
}

CoordinateVector::CoordinateVector(const CoordinateVector &vector)
    : Matrix(MatrixStaticStorage<4, 1>::getNewPooled(), true)
{
    for (int i = 0; i < getRows(); ++i)
        storage->get(i, 0) = vector.storage->get(i, 0);
}

CoordinateVector::CoordinateVector(double v1, double v2, double v3, double w)
    : Matrix(MatrixStaticStorage<4, 1>::getNewPooled(), true)
{
    storage->get(0, 0) = v1;
    storage->get(1, 0) = v2;
    storage->get(2, 0) = v3;
    storage->get(3, 0) = w;
}

CoordinateVector &CoordinateVector::operator=(const CoordinateVector &cv)
{
    if (this == &cv)
        return *this;

    if (storage == nullptr || getRows() != cv.getRows() || getCols() != cv.getCols())
        storage = MatrixStaticStorage<4, 1>::getNewPooled();

    for (int i = 0; i < getRows(); ++i)
    {
        for (int j = 0; j < getCols(); ++j)
            storage->get(i, j) = cv.storage->get(i, j);
    }

    return *this;
}

const double CoordinateVector::cGetX() const
{
    return storage->get(0, 0);
}

const double CoordinateVector::cGetY() const
{
    return storage->get(1, 0);
}

const double CoordinateVector::cGetZ() const
{
    return storage->get(2, 0);
}

const double CoordinateVector::cGetW() const
{
    return storage->get(3, 0);
}

double &CoordinateVector::getX()
{
    return storage->get(0, 0);
}

double &CoordinateVector::getY()
{
    return storage->get(1, 0);
}

double &CoordinateVector::getZ()
{
    return storage->get(2, 0);
}

double &CoordinateVector::getW()
{
    return storage->get(3, 0);
}

double CoordinateVector::scalarMultiply(const CoordinateVector &vector)
{
    if (this->getCols() != vector.getCols())
        throw std::logic_error("Can't execute scalar multiply");

    double result = 0;

    for (int i = 0; i < this->getRows(); ++i)
        result += storage->get(i, 0) * vector.storage->get(i, 0);

    return result;
}

const double CoordinateVector::getLength()
{
    if (length.has_value())
        return *length;

    length = sqrt(pow(storage->get(0, 0), 2) + pow(storage->get(1, 0), 2) + pow(storage->get(2, 0), 2));

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
