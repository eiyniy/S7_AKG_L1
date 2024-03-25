#include <Matrix.hpp>
#include <Point.hpp>
#include <SFML/Graphics.hpp>

class Converter
{
public:
    static Vector<4> colorToMatrix(const sf::Color &value);

    static sf::Color matrixToColor(const Vector<4> &value);
};
