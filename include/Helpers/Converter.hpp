#include <Matrix.hpp>
#include <DrawableVertex.hpp>
#include <Point.hpp>
#include <SFML/Graphics.hpp>

class Converter {
public:
    static DrawableVertex matrixToDrawableVertex(
            const Matrix<4, 1> &value,
            double w,
            bool isWNegative);

    static Point drawableVertexToPoint(const DrawableVertex &value);

    static Matrix<4, 1> drawableVertexToMatrix(const DrawableVertex &value);

    static Matrix<4, 1> colorToMatrix(const sf::Color &value);

    static sf::Color matrixToColor(const Matrix<4, 1> &value);
};
