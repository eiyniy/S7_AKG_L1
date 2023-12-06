#pragma once

#include <vector>
#include <DrawableVertex.hpp>
#include <Polygon.hpp>
#include <SFML/Config.hpp>
#include <SFML/Graphics/Color.hpp>
#include <Point.hpp>
#include <BaseLightingModel.hpp>
#include <NormalVertex.hpp>
#include <BaseLightSource.hpp>

class BarycentricRasterizer {
public:
    static void rasterize(
            const Polygon &polygon,
            const std::vector<DrawableVertex> &drawableVertices,
            const std::vector<NormalVertex> &normalVertices,
            const sf::Color &color,
            double *depthBuffer,
            int xSize,
            sf::Uint8 *pixels,
            ShadingModel shadingModel,
            const BaseLightingModel *lightingModel,
            const BaseLightSource *lightSource);

private:
    static std::pair<Point, Point> findWindowingRectangle(
            const Polygon &polygon,
            const std::vector<DrawableVertex> &vertices);

    static sf::Color getShadedColor(
            const sf::Color &color,
            const Polygon &polygon,
            const std::vector<NormalVertex> &normalVertices,
            const Matrix<4, 1> &lightDirection,
            ShadingModel shadingModel,
            const BaseLightingModel *lightingModel);

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
