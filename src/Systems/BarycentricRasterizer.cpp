#include <iostream>
#include <BarycentricRasterizer.hpp>
#include <cstring>

void BarycentricRasterizer::rasterize(
        const Polygon &polygon,
        const std::vector<DrawableVertex> &drawableVertices,
        const std::vector<NormalVertex> &normalVertices,
        const sf::Color &color,
        double *depthBuffer,
        const int xSize,
        sf::Uint8 *pixels,
        ShadingModel shadingModel,
        const BaseLightingModel *lightingModel,
        const BaseLightSource *lightSource) {
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
    const int minX = windowingRectangle.first.cGetX();
    const int minY = windowingRectangle.first.cGetY();
    const int maxX = windowingRectangle.second.cGetX();
    const int maxY = windowingRectangle.second.cGetY();

//    std::cout << "HERE" << std::endl;
    // TODO: Передавать сюда не drawable Vertex а double

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

                    const auto shadedColor = getShadedColor(
                            color,
                            polygon,
                            normalVertices,
                            lightSource->getLightDirection({0, 0, 0}),
                            shadingModel,
                            lightingModel);

                    std::memcpy(
                            pixels + (4 * (i * xSize + j)),
                            &shadedColor,
                            4);
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

    auto minX = (int) std::floor(firstVertex.CGetX());
    auto maxX = (int) std::ceil(firstVertex.CGetX());
    auto minY = (int) std::floor(firstVertex.CGetY());
    auto maxY = (int) std::ceil(firstVertex.CGetY());

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

sf::Color BarycentricRasterizer::getShadedColor(
        const sf::Color &color,
        const Polygon &polygon,
        const std::vector<NormalVertex> &normalVertices,
        const Matrix<4, 1> &lightDirection,
        ShadingModel shadingModel,
        const BaseLightingModel *lightingModel) {
    sf::Color result;

    switch (shadingModel) {
        case Flat: {
            const auto id0 = polygon.cGetVertexIds(0).cGetNormalVertexId();
            const auto id1 = polygon.cGetVertexIds(1).cGetNormalVertexId();
            const auto id2 = polygon.cGetVertexIds(2).cGetNormalVertexId();

            if (!id0.has_value() || !id1.has_value() || !id2.has_value())
                throw std::runtime_error("Can not get Normal");

            const auto n0 = normalVertices.at(*id0 - 1);
            const auto n1 = normalVertices.at(*id1 - 1);
            const auto n2 = normalVertices.at(*id2 - 1);

            const auto invLightDir = lightDirection * -1;

            const auto intensity0 = lightingModel->getLightIntensity(n0, invLightDir);
            const auto intensity1 = lightingModel->getLightIntensity(n1, invLightDir);
            const auto intensity2 = lightingModel->getLightIntensity(n2, invLightDir);

            const auto averageIntensity = (intensity0 + intensity1 + intensity2) / 3;

            result = sf::Color(color.r * averageIntensity, color.g * averageIntensity, color.b * averageIntensity);

            break;
        }
    }

    return result;
}
