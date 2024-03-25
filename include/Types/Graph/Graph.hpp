#pragma once

#include <memory>
#include <vector>
#include <Triangle.hpp>
#include <Matrix.hpp>
#include <Point.hpp>

class Graph
{
private:
    std::vector<std::vector<bool>> adjacencyMatrix;
    std::vector<const Vector<4> *> nodes;

    int findNearestVertexId(const Point &pos) const;

    // TODO: Move this shit from here
    static double findDistance(
        const Point &v1,
        const Point &v2);

public:
    Graph();

    Graph(
        const std::vector<Vector<4>> &drawableVertices,
        const std::vector<Triangle> &polygons);

    std::unique_ptr<std::vector<int>> getAffectedVerticesIds(
        const Point &mousePos,
        const int radius) const;
};

// TODO: Move to Point
inline double Graph::findDistance(const Point &v1, const Point &v2)
{
    return std::sqrt(std::pow(std::fabs(v2.cGetX() - v1.cGetX()), 2) + std::pow(std::fabs(v2.cGetY() - v1.cGetY()), 2));
}
