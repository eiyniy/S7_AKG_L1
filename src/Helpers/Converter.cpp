#include <Converter.hpp>

DrawableVertex Converter::matrixToDrawableVertex(
    const Matrix<4, 1> &value,
    double w,
    const bool isWNegative)
{
    return {
        value.cGetX(),
        value.cGetY(),
        w,
        isWNegative};
}

Point Converter::drawableVertexToPoint(const DrawableVertex &value)
{
    return {(int)std::round(value.CGetX()), (int)std::round(value.CGetY())};
}

Matrix<4, 1> Converter::drawableVertexToMatrix(const DrawableVertex &value)
{
    return {value.CGetX(), value.CGetY(), value.CGetZ()};
}

Matrix<4, 1> Converter::colorToMatrix(const sf::Color &value)
{
    return {value.r, value.g, value.b};
}

sf::Color Converter::matrixToColor(const Matrix<4, 1> &value)
{
    return {
        std::min(value.cGetX(), 255.0),
        std::min(value.cGetY(), 255.0),
        std::min(value.cGetZ(), 255.0)};
}
