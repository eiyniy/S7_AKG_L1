#include <Matrix.hpp>
#include <Point.hpp>
#include <SFML/Graphics.hpp>

class Converter
{
public:
    static Matrix<4, 1> colorToMatrix(const sf::Color &value);

    static sf::Color matrixToColor(const Matrix<4, 1> &value);
};
