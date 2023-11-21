#include <Converter.hpp>

Matrix<4, 1> Converter::vertexToMatrix(const Vertex &value) {
    return {value.cGetX(), value.cGetY(), value.cGetZ(), value.cGetW()};
}

DrawableVertex Converter::matrixToDrawableVertex(
        const Matrix<4, 1> &value,
        const bool isVisible,
        const bool isWNegative) {
    return {
            static_cast<int>(std::round(value.cGetX())),
            static_cast<int>(std::round(value.cGetY())),
            isVisible,
            isWNegative};
}

Point Converter::drawableVertexToPoint(const DrawableVertex &value) {
    return {value.CGetX(), value.CGetY()};
}
