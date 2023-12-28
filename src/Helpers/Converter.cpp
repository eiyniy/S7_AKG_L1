#include <Converter.hpp>

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
