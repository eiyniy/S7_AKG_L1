#include <Matrix.hpp>
#include <BaseVertex.hpp>
#include <Vertex.hpp>
#include <Point.hpp>

class Converter
{
public:
    static Matrix<4, 1> vertexToMatrix(const Vertex &value);
    static Vertex matrixToVertex(
        const Matrix<4, 1> &value,
        const bool isOutOfScreen);
    static Point vertexToPoint(const Vertex &value);
};
