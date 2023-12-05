#include <Converter.hpp>

Matrix<4, 1> Converter::vertexToMatrix(const Vertex &value) {
    return {value.cGetX(), value.cGetY(), value.cGetZ(), value.cGetW()};
}

DrawableVertex Converter::matrixToDrawableVertex(
        const Matrix<4, 1> &value,
        double w,
        const bool isVisible,
        const bool isWNegative) {
    return {
            (int) (std::round(value.cGetX())),
            (int) (std::round(value.cGetY())),
            w,
            isVisible,
            isWNegative};
}

Point Converter::drawableVertexToPoint(const DrawableVertex &value) {
    return {value.CGetX(), value.CGetY()};
}
