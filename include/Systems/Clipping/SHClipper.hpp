#pragma once

#include <Point.hpp>
#include <Polygon.hpp>
#include <Vertex.hpp>
#include <DrawableVertex.hpp>

class SHClipper {
public:
    static void clip(
            Polygon &polygon,
            std::vector<DrawableVertex> &vertices,
            const std::vector<Point> &clipper);

private:
    static void clipByEdge(
            Polygon &polygon,
            std::vector<DrawableVertex> &vertices,
            const Point &edgeP1, const Point &edgeP2);

    static Point findIntersection(
            const Point &line1P1, const Point &line1P2,
            const Point &line2P1, const Point &line2P2);

    static int findPointRelPos(
            const Point &p, const Point &edgeP1, const Point &edgeP2);
};

inline int SHClipper::findPointRelPos(
        const Point &p, const Point &edgeP1, const Point &edgeP2) {
    return (p.cGetX() - edgeP1.cGetX()) * (edgeP2.cGetY() - edgeP1.cGetY()) -
           (p.cGetY() - edgeP1.cGetY()) * (edgeP2.cGetX() - edgeP1.cGetX());
}
