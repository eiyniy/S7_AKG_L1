#include <iostream>
#include <BarycentricRasterizer.hpp>
#include <cstring>

void BarycentricRasterizer::rasterize(
        const Polygon &polygon,
        const std::vector<DrawableVertex> &drawableVertices,
        double *depthBuffer,
        const int xSize,
        sf::Uint8 *pixels,
        sf::Color *color) {
    const auto &a = drawableVertices.at(polygon.cGetVertexIds(0).cGetVertexId() - 1);
    const auto &b = drawableVertices.at(polygon.cGetVertexIds(1).cGetVertexId() - 1);
    const auto &c = drawableVertices.at(polygon.cGetVertexIds(2).cGetVertexId() - 1);

    const auto invAZ = 1 / a.CGetZ();
    const auto invBZ = 1 / b.CGetZ();
    const auto invCZ = 1 / c.CGetZ();

    const auto area = edgeFunction(a, b, c);

    if (area == 0)
        return;

    const auto windowingRectangle = findWindowingRectangle(polygon, drawableVertices);
    const auto minX = windowingRectangle.first.cGetX();
    const auto minY = windowingRectangle.first.cGetY();
    const auto maxX = windowingRectangle.second.cGetX();
    const auto maxY = windowingRectangle.second.cGetY();

    auto w0 = edgeFunction(b, c, {minX, minY, 0}) / area;
    auto w1 = edgeFunction(c, a, {minX, minY, 0}) / area;
    auto w2 = edgeFunction(a, b, {minX, minY, 0}) / area;

    const auto w0XLast = edgeFunction(b, c, {maxX, minY, 0}) / area;
    const auto w1XLast = edgeFunction(c, a, {maxX, minY, 0}) / area;
    const auto w2XLast = edgeFunction(a, b, {maxX, minY, 0}) / area;

    const auto w0YLast = edgeFunction(b, c, {minX, maxY, 0}) / area;
    const auto w1YLast = edgeFunction(c, a, {minX, maxY, 0}) / area;
    const auto w2YLast = edgeFunction(a, b, {minX, maxY, 0}) / area;

    const auto w0XInterval = w0XLast - w0;
    const auto w1XInterval = w1XLast - w1;
    const auto w2XInterval = w2XLast - w2;

    const auto w0YInterval = w0YLast - w0;
    const auto w1YInterval = w1YLast - w1;
    const auto w2YInterval = w2YLast - w2;

    const auto xInterval = maxX - minX;
    const auto yInterval = maxY - minY;

    const auto w0XStep = w0XInterval / xInterval;
    const auto w1XStep = w1XInterval / xInterval;
    const auto w2XStep = w2XInterval / xInterval;

    const auto w0YStep = w0YInterval / yInterval;
    const auto w1YStep = w1YInterval / yInterval;
    const auto w2YStep = w2YInterval / yInterval;

    for (int i = minY; i <= maxY; ++i) {
        for (int j = minX; j <= maxX; ++j) {
            if (w0 >= 0 && w1 >= 0 && w2 >= 0) {
                const auto z = 1 / (invAZ * w0 + invBZ * w1 + invCZ * w2);
//                const auto z = a.CGetZ() * w0 + b.CGetZ() * w1 + c.CGetZ() * w2;

                if (z < depthBuffer[i * xSize + j]) {
                    depthBuffer[i * xSize + j] = z;
                    std::memcpy(pixels + (4 * (i * xSize + j)), color, 4);
                }
            }

            w0 += w0XStep;
            w1 += w1XStep;
            w2 += w2XStep;
        }

        w0 -= w0XStep * (xInterval + 1);
        w1 -= w1XStep * (xInterval + 1);
        w2 -= w2XStep * (xInterval + 1);

        w0 += w0YStep;
        w1 += w1YStep;
        w2 += w2YStep;
    }
}

std::pair<Point, Point> BarycentricRasterizer::findWindowingRectangle(
        const Polygon &polygon,
        const std::vector<DrawableVertex> &vertices) {
    const auto &firstVertex = vertices.at(polygon.cGetVertexIds(0).cGetVertexId() - 1);

    auto minX = firstVertex.CGetX();
    auto maxX = firstVertex.CGetX();
    auto minY = firstVertex.CGetY();
    auto maxY = firstVertex.CGetY();

    const auto size = polygon.cGetVertexIdsCount();
    for (int i = 0; i < size; ++i) {
        const auto &vertex = vertices.at(polygon.cGetVertexIds(i).cGetVertexId() - 1);

        if (vertex.CGetX() < minX) minX = vertex.CGetX();
        else if (vertex.CGetX() > maxX) maxX = vertex.CGetX();
        if (vertex.CGetY() < minY) minY = vertex.CGetY();
        else if (vertex.CGetY() > maxY) maxY = vertex.CGetY();
    }

    return {{minX, minY},
            {maxX, maxY}};
}
