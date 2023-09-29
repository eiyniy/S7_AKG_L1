#include <CoordinateVector.hpp>
#include <MatrixStaticStorage.hpp>
#include <Timer.hpp>
#include <iostream>

CoordinateVector::CoordinateVector()
    : Matrix(MatrixStaticStorage<1, 4>::getNewPooled()) {}

CoordinateVector::CoordinateVector(const CoordinateVector &vector)
    : Matrix(MatrixStaticStorage<1, 4>::getNewPooled())
{
    Timer::start();

    for (int j = 0; j < getCols(); ++j)
        storage->get(0, j) = vector.storage->get(0, j);

    Timer::stop();
}

CoordinateVector::CoordinateVector(double v1, double v2, double v3, double w)
    : Matrix(MatrixStaticStorage<1, 4>::getNewPooled())
{
    storage->get(0, 0) = v1;
    storage->get(0, 1) = v2;
    storage->get(0, 2) = v3;
    storage->get(0, 3) = w;
}

CoordinateVector CoordinateVector::fromCoordinats(double v1, double v2, double v3, double w)
{
    auto cv = CoordinateVector();

    cv.storage->get(0, 0) = v1;
    cv.storage->get(0, 1) = v2;
    cv.storage->get(0, 2) = v3;
    cv.storage->get(0, 2) = v3;

    return cv;
}

CoordinateVector &CoordinateVector::operator*=(const CoordinateVector &cv)
{
    CoordinateVector temp;

    temp.storage->get(0, 0) = (getY() * cv.getZ()) - (getZ() * cv.getY());
    temp.storage->get(0, 1) = (getZ() * cv.getX()) - (getX() * cv.getZ());
    temp.storage->get(0, 2) = (getX() * cv.getY()) - (getY() * cv.getX());
    temp.storage->get(0, 3) = 1;

    return (*this = temp);
}

double CoordinateVector::getX() const
{
    return storage->get(0, 0);
}

double CoordinateVector::getY() const
{
    return storage->get(0, 1);
}

double CoordinateVector::getZ() const
{
    return storage->get(0, 2);
}

double CoordinateVector::getW() const
{
    return storage->get(0, 3);
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
    if (length.has_value())
        return length.value();

    length = sqrt(pow(storage->get(0, 0), 2) + pow(storage->get(0, 1), 2) + pow(storage->get(0, 2), 2));

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

void CoordinateVector::convert(
    CoordinateVector xAxis,
    CoordinateVector yAxis,
    CoordinateVector zAxis,
    CoordinateVector translation)
{
    Matrix multiplier{MatrixStaticStorage<4, 4>::getNewPooled()};

    multiplier.getValue(0, 0) = xAxis.getX();
    multiplier.getValue(0, 1) = yAxis.getX();
    multiplier.getValue(0, 2) = zAxis.getX();
    multiplier.getValue(0, 3) = translation.getX();

    multiplier.getValue(1, 0) = xAxis.getY();
    multiplier.getValue(1, 1) = yAxis.getY();
    multiplier.getValue(1, 2) = zAxis.getY();
    multiplier.getValue(1, 3) = translation.getY();

    multiplier.getValue(2, 0) = xAxis.getZ();
    multiplier.getValue(2, 1) = yAxis.getZ();
    multiplier.getValue(2, 2) = zAxis.getZ();
    multiplier.getValue(2, 3) = translation.getZ();

    multiplier.getValue(3, 0) = xAxis.getW();
    multiplier.getValue(3, 1) = yAxis.getW();
    multiplier.getValue(3, 2) = zAxis.getW();
    multiplier.getValue(3, 3) = translation.getW();

    *this *= multiplier;
}

void CoordinateVector::moveConvert(const CoordinateVector &translation)
{
    convert(
        {1, 0, 0, 0},
        {0, 1, 0, 0},
        {0, 0, 1, 0},
        translation);
}

void CoordinateVector::scaleConvert(const CoordinateVector &scale)
{
    convert(
        {scale.getX(), 0, 0, 0},
        {0, scale.getY(), 0, 0},
        {0, 0, scale.getZ(), 0},
        {0, 0, 0, 1});
}

void CoordinateVector::rotateConvert(AxisName axis, double angle)
{
    switch (axis)
    {
    case AxisName::X:
        convert(
            {1, 0, 0, 0},
            {0, cos(angle), sin(angle), 0},
            {0, -sin(angle), cos(angle), 0},
            {0, 0, 0, 1});
        break;
    case AxisName::Y:
        convert(
            {cos(angle), 0, -sin(angle), 0},
            {0, 1, 0, 0},
            {sin(angle), 0, cos(angle), 0},
            {0, 0, 0, 1});
        break;
    case AxisName::Z:
        convert(
            {cos(angle), 0, -sin(angle), 0},
            {0, 1, 0, 0},
            {sin(angle), 0, cos(angle), 0},
            {0, 0, 0, 1});
        break;
    }
}

void CoordinateVector::toObserverConvert(
    const CoordinateVector &eye,
    const CoordinateVector &target,
    const CoordinateVector &up)
{

    CoordinateVector zAxis = eye - target;
    CoordinateVector xAxis = up * zAxis;
    CoordinateVector yAxis = zAxis * xAxis;

    xAxis = xAxis.getNormalized();
    yAxis = yAxis.getNormalized();
    zAxis = zAxis.getNormalized();

    convert(
        {
            xAxis.getX(),
            yAxis.getX(),
            zAxis.getX(),
            0,
        },
        {
            xAxis.getY(),
            yAxis.getY(),
            zAxis.getY(),
            0,
        },
        {
            xAxis.getZ(),
            yAxis.getZ(),
            zAxis.getZ(),
            0,
        },
        {
            -xAxis.scalarMultiply(eye),
            -yAxis.scalarMultiply(eye),
            -zAxis.scalarMultiply(eye),
            1,
        });
}

void CoordinateVector::toProjectionConvert(double fov, double aspect, double zFar, double zNear)
{
    convert(
        {
            1.0 / (aspect * tan(fov / 2)),
            0,
            0,
            0,
        },
        {
            0,
            1.0 / tan(fov / 2),
            0,
            0,
        },
        {
            0,
            0,
            zFar / (zNear - zFar),
            -1,
        },
        {
            0,
            0,
            zNear * zFar / (zNear - zFar),
            0,
        });
}

void CoordinateVector::toViewerConvert(double width, double height, double xMin, double yMin)
{
    convert(
        {
            width / 2,
            0,
            0,
            0,
        },
        {
            0,
            -height / 2,
            0,
            0,
        },
        {
            0,
            0,
            1,
            0,
        },
        {
            xMin + width / 2,
            yMin + height / 2,
            0,
            1,
        });
}

void CoordinateVector::log()
{
    std::cout << getX() << " " << getY() << " " << getZ() << " " << getW() << " " << std::endl;
}

CoordinateVector
operator*(const CoordinateVector &cv1, const CoordinateVector &cv2)
{
    CoordinateVector temp(cv1);
    return (temp *= cv2);
}
