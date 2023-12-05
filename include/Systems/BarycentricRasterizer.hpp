#pragma once

#include <vector>
#include <DrawableVertex.hpp>
#include <Polygon.hpp>
#include <SFML/Config.hpp>
#include <SFML/Graphics/Color.hpp>
#include "Point.hpp"

class BarycentricRasterizer {
public:
    static void rasterize(
            const Polygon &polygon,
            const std::vector<DrawableVertex> &drawableVertices,
            double *depthBuffer,
            int xSize,
            sf::Uint8 *pixels,
            sf::Color *color);

private:
    static std::pair<Point, Point> findWindowingRectangle(
            const Polygon &polygon,
            const std::vector<DrawableVertex> &vertices);

    static double edgeFunction(
            const DrawableVertex &a,
            const DrawableVertex &b,
            const DrawableVertex &c);
};

inline double BarycentricRasterizer::edgeFunction(
        const DrawableVertex &a,
        const DrawableVertex &b,
        const DrawableVertex &c) {
    return (c.CGetX() - a.CGetX()) * (b.CGetY() - a.CGetY()) -
           (c.CGetY() - a.CGetY()) * (b.CGetX() - a.CGetX());
}
