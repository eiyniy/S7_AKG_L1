#include <Matrix.hpp>
#include <BaseVertex.hpp>
#include <Vertex.hpp>
#include <DrawableVertex.hpp>
#include <Point.hpp>

class Converter {
public:
    static Matrix<4, 1> vertexToMatrix(const Vertex &value);

    static DrawableVertex matrixToDrawableVertex(
            const Matrix<4, 1> &value,
            double w,
            bool isVisible,
            bool isWNegative);

    static Point drawableVertexToPoint(const DrawableVertex &value);
};
