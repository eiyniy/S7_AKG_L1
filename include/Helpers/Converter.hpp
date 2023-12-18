#include <Matrix.hpp>
#include <BaseVertex.hpp>
#include <Vertex.hpp>
#include <DrawableVertex.hpp>
#include <Point.hpp>
#include <NormalVertex.hpp>

class Converter {
public:
    static Matrix<4, 1> vertexToMatrix(const Vertex &value);

    static Matrix<4, 1> nVertexToMatrix(const NormalVertex &value);

    static DrawableVertex matrixToDrawableVertex(
            const Matrix<4, 1> &value,
            double w,
            bool isWNegative);

    static Vertex matrixToVertex(
            const Matrix<4, 1> &value,
            double w);

    static Point drawableVertexToPoint(const DrawableVertex &value);

    static Matrix<4, 1> drawableVertexToMatrix(const DrawableVertex &value);

    static Matrix<4, 1> normalVertexToMatrix(const NormalVertex &value);
};
