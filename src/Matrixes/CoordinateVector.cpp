#include <CoordinateVector.hpp>
#include <MatrixStaticStorage.hpp>
#include <Timer.hpp>
#include <Vertex.hpp>
#include <Pool.hpp>
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

// /*
CoordinateVector &CoordinateVector::operator*=(const CoordinateVector &cv)
{
    CoordinateVector temp;

    temp.storage->get(0, 0) = (cGetY() * cv.cGetZ()) - (cGetZ() * cv.cGetY());
    temp.storage->get(1, 0) = (cGetZ() * cv.cGetX()) - (cGetX() * cv.cGetZ());
    temp.storage->get(2, 0) = (cGetX() * cv.cGetY()) - (cGetY() * cv.cGetX());
    temp.storage->get(3, 0) = 1;

    return (*this = temp);
}
// */

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

    double result;

    for (int i = 0; i < this->getRows(); ++i)
        result += storage->get(i, 0) * vector.storage->get(i, 0);

    return result;
}

const double CoordinateVector::getLength()
{
    if (length.has_value())
        return length.value();

    length = sqrt(pow(storage->get(0, 0), 2) + pow(storage->get(1, 0), 2) + pow(storage->get(2, 0), 2));

    return length.value();
}

CoordinateVector CoordinateVector::getNormalized()
{
    if (getLength() != 0)
    {
        auto matrix = *this * (1 / getLength());
        return *static_cast<CoordinateVector *>(&matrix);
    }
    else
        return CoordinateVector();
}

void CoordinateVector::log()
{
    std::cout << cGetX() << " " << cGetY() << " " << cGetZ() << " " << cGetW() << " " << std::endl;
}

CoordinateVector
operator*(const CoordinateVector &cv1, const CoordinateVector &cv2)
{
    CoordinateVector temp(cv1);
    return (temp *= cv2);
}
