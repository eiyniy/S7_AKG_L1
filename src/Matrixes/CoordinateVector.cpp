#include <CoordinateVector.hpp>
#include <MatrixStaticStorage.hpp>
#include <Timer.hpp>

CoordinateVector::CoordinateVector()
    // : Matrix(new MatrixStaticStorage<1, 3>()) {}
    : Matrix(MatrixStaticStorage<1, 3>::getNewPooled())
{
}

CoordinateVector::CoordinateVector(const CoordinateVector &vector)
    : Matrix(MatrixStaticStorage<1, 3>::getNewPooled())
{
    Timer::start();

    for (int j = 0; j < getCols(); ++j)
        storage->get(0, j) = vector.storage->get(0, j);

    Timer::stop();
}

CoordinateVector::CoordinateVector(double v1, double v2, double v3)
    : Matrix(MatrixStaticStorage<1, 3>::getNewPooled())
{
    storage->get(0, 0) = v1;
    storage->get(0, 1) = v2;
    storage->get(0, 2) = v3;
}

double CoordinateVector::getX()
{
    return storage->get(0, 0);
}

double CoordinateVector::getY()
{
    return storage->get(0, 1);
}

double CoordinateVector::getZ()
{
    return storage->get(0, 2);
}

double CoordinateVector::scalarMultiply(const CoordinateVector &vector)
{
    if (this->getCols() != vector.getCols())
        throw std::logic_error("Can't execute scalar multiply");

    double result;

    for (int i = 0; i < this->getCols(); ++i)
        result += storage->get(0, i) * vector.storage->get(0, i);

    return result;
}

const double CoordinateVector::getLength()
{
    if (getRows() != 1 || getCols() != 3)
        throw std::logic_error("Can't get lenght");

    if (length.has_value())
        return length.value();

    length = sqrt(pow(storage->get(0, 0), 2) + pow(storage->get(0, 1), 2) + pow(storage->get(0, 2), 2));

    return length.value();
}

CoordinateVector CoordinateVector::getNormalized()
{
    auto matrix = *this * (1 / getLength());
    return *static_cast<CoordinateVector *>(&matrix);
}
