#include <Converter.hpp>

Matrix<4, 1> Converter::cVectorToMatrix(const CoordinateVector &value)
{
    auto mx = Matrix<4, 1>();

    mx.getValue(0, 0) = value.cGetX();
    mx.getValue(1, 0) = value.cGetY();
    mx.getValue(2, 0) = value.cGetZ();
    mx.getValue(3, 0) = value.cGetW();

    return mx;
}

CoordinateVector Converter::vertexToCVector(const Vertex &value)
{
    return CoordinateVector(value.cGetX(), value.cGetY(), value.cGetZ(), value.cGetW());
}

CoordinateVector Converter::matrixToCVector(const Matrix<4, 1> &value)
{
    if (value.cols != 1 || value.rows != 4)
        throw std::invalid_argument("Could not convert value");

    return CoordinateVector(
        value.cGetValue(0, 0),
        value.cGetValue(1, 0),
        value.cGetValue(2, 0),
        value.cGetValue(3, 0));
}

Vertex Converter::cVectorToVertex(const CoordinateVector &value, const bool isOutOfScreen, const bool isWNegative)
{
    return Vertex(
        value.cGetX(),
        value.cGetY(),
        value.cGetZ(),
        value.cGetW(),
        isOutOfScreen,
        isWNegative);
}
