#pragma once

#include <Polygon.hpp>
#include <vector>
#include <Vertex.hpp>
#include <list>
#include <Converter.hpp>

class EarClipper
{
private:
        static double area(
            const Matrix<4, 1> &v0, const Matrix<4, 1> &v1, const Matrix<4, 1> &v2);

        static bool isPointsInside(
            const std::pair<Matrix<4, 1>, VertexIds> &v0,
            const std::pair<Matrix<4, 1>, VertexIds> &v1,
            const std::pair<Matrix<4, 1>, VertexIds> &v2,
            const std::vector<std::pair<Matrix<4, 1>, VertexIds>> &polygonVertices);

        static Polygon clipEar(
            std::vector<std::pair<Matrix<4, 1>, VertexIds>> &polygonVertices);

        static bool isConvexVertex(
            const Matrix<4, 1> &vertex,
            const Matrix<4, 1> &prevVertex,
            const Matrix<4, 1> &nextVertex);

public:
        static std::vector<Polygon> triangulate(
            std::vector<std::pair<Matrix<4, 1>, VertexIds>> &polygonVertices);

        static std::vector<Polygon> triangulate(
            const std::vector<VertexIds> &indexes,
            const std::vector<Matrix<4, 1>> &vertices);
};

inline double EarClipper::area(
    const Matrix<4, 1> &v0, const Matrix<4, 1> &v1, const Matrix<4, 1> &v2)
{
        const auto vec1 = v1 - v0;
        const auto vec2 = v2 - v0;

        const auto crossProduct = vec1.vectorMultiply(vec2);
        const auto sin = crossProduct.getLength() / (vec1.getLength() * vec2.getLength());

        return std::fabs(0.5 * vec1.getLength() * vec2.getLength() * sin);

        //    return std::fabs((
        //                             v1.cGetX() * (v2.cGetY() - v3.cGetY()) +
        //                             v2.cGetX() * (v3.cGetY() - v1.cGetY()) +
        //                             v3.cGetX() * (v1.cGetY() - v2.cGetY())) / 2.0);
}
