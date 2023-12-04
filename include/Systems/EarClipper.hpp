#pragma once

#include <Polygon.hpp>
#include <vector>
#include <Vertex.hpp>
#include <list>
#include "Converter.hpp"

class EarClipper {
private:
    static double area(
            const Vertex &v1, const Vertex &v2, const Vertex &v3);

    static bool isPointsInside(
            const std::pair<Vertex, VertexIds> &v1,
            const std::pair<Vertex, VertexIds> &v2,
            const std::pair<Vertex, VertexIds> &v3,
            const std::vector<std::pair<Vertex, VertexIds>> &polygonVertices);

    static Polygon clipEar(
            std::vector<std::pair<Vertex, VertexIds>> &polygonVertices);

    static bool isConvexVertex(
            const Vertex &vertex,
            const Vertex &prevVertex,
            const Vertex &nextVertex);

public:
    static std::vector<Polygon> triangulate(
            std::vector<std::pair<Vertex, VertexIds>> &polygonVertices);

    static std::vector<Polygon> triangulate(
            const std::vector<VertexIds> &indexes,
            const std::vector<Vertex> &vertices);
};

inline double EarClipper::area(
        const Vertex &v1, const Vertex &v2, const Vertex &v3) {
    const auto matrix = Converter::vertexToMatrix(v1);
    const auto prevMatrix = Converter::vertexToMatrix(v2);
    const auto nextMatrix = Converter::vertexToMatrix(v3);

    const auto vec1 = prevMatrix - matrix;
    const auto vec2 = nextMatrix - matrix;

    const auto crossProduct = vec1.vectorMultiply(vec2);
    const auto sin = crossProduct.getLength() / (vec1.getLength() * vec2.getLength());

    return std::fabs(0.5 * vec1.getLength() * vec2.getLength() * sin);

//    return std::fabs((
//                             v1.cGetX() * (v2.cGetY() - v3.cGetY()) +
//                             v2.cGetX() * (v3.cGetY() - v1.cGetY()) +
//                             v3.cGetX() * (v1.cGetY() - v2.cGetY())) / 2.0);
}
