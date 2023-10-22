#include <Converter.hpp>

Matrix<4, 1> Converter::vertexToMatrix(const Vertex &value)
{
    return Matrix<4, 1>(value.cGetX(), value.cGetY(), value.cGetZ(), value.cGetW());
}

Vertex Converter::matrixToVertex(
    const Matrix<4, 1> &value,
    const bool isOutOfScreen)
{
    return Vertex(
        value.cGetX(),
        value.cGetY(),
        value.cGetZ(),
        value.cGetW(),
        isOutOfScreen);
}

Point Converter::vertexToPoint(const Vertex &value)
{
    return Point(value.cGetX(), value.cGetY());
}
