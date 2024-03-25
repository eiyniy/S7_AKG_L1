#include <Converter.hpp>

Vector<4> Converter::colorToMatrix(const sf::Color &value)
{
    return {value.r, value.g, value.b};
}

sf::Color Converter::matrixToColor(const Vector<4> &value)
{
    return {
        std::min(value.cGetX(), 255.0),
        std::min(value.cGetY(), 255.0),
        std::min(value.cGetZ(), 255.0)};
}
