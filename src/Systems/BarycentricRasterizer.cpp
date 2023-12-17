#include <BarycentricRasterizer.hpp>
#include <Converter.hpp>
#include <iostream>

BarycentricRasterizer::BarycentricRasterizer(
    const BaseLightingModel *_lightingModel,
    const BaseLightSource *_lightSource,
    const ShadingModel _shadingModel,
    const int _xSize,
    void (*_drawPixel)(sf::Uint8 *pixels, const int, const sf::Color *),
    sf::Uint8 *_pixels,
    double *_depthBuffer)
    : lightingModel(_lightingModel),
      lightSource(_lightSource),
      shadingModel(_shadingModel),
      xSize(_xSize),
      drawPixel(_drawPixel),
      pixels(_pixels),
      depthBuffer(_depthBuffer) {}

void BarycentricRasterizer::rasterize(
    Polygon &polygon,
    const std::vector<Matrix<4, 1>> &vertices,
    const std::vector<DrawableVertex> &drawableVertices,
    const sf::Color &color)
{
    const auto &a = drawableVertices.at(polygon.cGetVertexIds(0).cGetVertexId() - 1);
    const auto &b = drawableVertices.at(polygon.cGetVertexIds(1).cGetVertexId() - 1);
    const auto &c = drawableVertices.at(polygon.cGetVertexIds(2).cGetVertexId() - 1);

    // const auto area = edgeFunction(a, b, c);
    // if (area == 0)
    //     return;
    // const auto invArea = 1 / area;

    // std::cout << a.CGetZ() << " " << b.CGetZ() << " " << c.CGetZ() << std::endl;

    const auto invAZ = 1 / a.CGetZ();
    const auto invBZ = 1 / b.CGetZ();
    const auto invCZ = 1 / c.CGetZ();

    const auto windowingRectangle = findWindowingRectangle(polygon, drawableVertices);
    const int minX = windowingRectangle.first.cGetX();
    const int minY = windowingRectangle.first.cGetY();
    const int maxX = windowingRectangle.second.cGetX();
    const int maxY = windowingRectangle.second.cGetY();

    //    /*
    //    const Matrix<4, 1> minMinVertex{(double) minX, (double) minY, 0};
    //    const Matrix<4, 1> maxMinVertex{(double) maxX, (double) minY, 0};
    //    const Matrix<4, 1> minMaxVertex{(double) minX, (double) maxY, 0};

    const DrawableVertex minMinVertex{minX, minY, 0};
    const DrawableVertex maxMinVertex{maxX, minY, 0};
    const DrawableVertex minMaxVertex{minX, maxY, 0};

    //    const auto v0 = bMatrix - aMatrix;
    //    const auto v1 = cMatrix - aMatrix;

    const DrawableVertex v0{b.CGetX() - a.CGetX(), b.CGetY() - a.CGetY(), 0};
    const DrawableVertex v1{c.CGetX() - a.CGetX(), c.CGetY() - a.CGetY(), 0};

    const auto invDen = 1 / (v0.CGetX() * v1.CGetY() - v1.CGetX() * v0.CGetY());

    double w0, w1, w2;

    // w0 = edgeFunction(b, c, minMinVertex) * invArea;
    // w1 = edgeFunction(c, a, minMinVertex) * invArea;
    // w2 = 1 - w0 - w1;

    calcBarycentric(minMinVertex, a, v0, v1, invDen, w0, w1, w2);
    //    calcBarycentric(minMinVertex, aMatrix, v0, v1, invDen, w0, w1, w2);
    //    calcBarycentric(minMinVertex, aMatrix, bMatrix, cMatrix, w0, w1, w2);

    double w0XLast, w1XLast, w2XLast;

    // w0XLast = edgeFunction(b, c, maxMinVertex) * invArea;
    // w1XLast = edgeFunction(c, a, maxMinVertex) * invArea;
    // w2XLast = 1 - w0XLast - w1XLast;

    calcBarycentric(maxMinVertex, a, v0, v1, invDen, w0XLast, w1XLast, w2XLast);
    //    calcBarycentric(maxMinVertex, aMatrix, v0, v1, invDen, w0XLast, w1XLast, w2XLast);
    //    calcBarycentric(maxMinVertex, aMatrix, bMatrix, cMatrix, w0XLast, w1XLast, w2XLast);

    double w0YLast, w1YLast, w2YLast;

    // w0YLast = edgeFunction(b, c, minMaxVertex) * invArea;
    // w1YLast = edgeFunction(c, a, minMaxVertex) * invArea;
    // w2YLast = 1 - w0YLast - w1YLast;

    calcBarycentric(minMaxVertex, a, v0, v1, invDen, w0YLast, w1YLast, w2YLast);
    //    calcBarycentric(minMaxVertex, aMatrix, v0, v1, invDen, w0YLast, w1YLast, w2YLast);
    //    calcBarycentric(minMaxVertex, aMatrix, bMatrix, cMatrix, w0YLast, w1YLast, w2YLast);

    const auto xInterval = maxX - minX;
    const auto yInterval = maxY - minY;

    const auto w0XStep = (w0XLast - w0) / xInterval;
    const auto w1XStep = (w1XLast - w1) / xInterval;
    const auto w2XStep = (w2XLast - w2) / xInterval;

    const auto w0RowStep = w0XStep * (xInterval + 1);
    const auto w1RowStep = w1XStep * (xInterval + 1);
    const auto w2RowStep = w2XStep * (xInterval + 1);

    const auto w0YStep = (w0YLast - w0) / yInterval - w0RowStep;
    const auto w1YStep = (w1YLast - w1) / yInterval - w1RowStep;
    const auto w2YStep = (w2YLast - w2) / yInterval - w2RowStep;

    // std::cout << "w0: " << w0  << ", w1: " << w1 << ", w2: " << w2 << std::endl;
    // std::cout << "w0XStep: " << w0XStep << ", w1XStep: " << w1XStep << ", w2XStep: " << w2XStep << std::endl;
    // std::cout << "w0YStep: " << w0YStep << ", w1YStep: " << w1YStep << ", w2YStep: " << w2YStep << std::endl;
    // std::cout << std::endl;

    const auto shadedColor = getShadedColor(
        color,
        polygon,
        vertices,
        lightSource->getLightDirection({0, 0, 0}),
        shadingModel,
        lightingModel);

    //    */
    for (int i = minY; i <= maxY; ++i, w0 += w0YStep, w1 += w1YStep, w2 += w2YStep)
    {
        for (int j = minX; j <= maxX; ++j, w0 += w0XStep, w1 += w1XStep, w2 += w2XStep)
        {
            // calcBarycentric({j, i, 0}, a, v0, v1, invDen, w0, w1, w2);

            if (w0 < 0 || w1 < 0 || w2 < 0)
                continue;

            // const auto z = a.CGetZ() * w0 + b.CGetZ() * w1 + c.CGetZ() * w2;
            const auto z = 1 / (invAZ * w0 + invBZ * w1 + invCZ * w2);
            const auto matrixPos = i * xSize + j;

            if (z >= depthBuffer[matrixPos])
                continue;

            depthBuffer[matrixPos] = z;
            // drawPixel(pixels, matrixPos, &polygon.color);
            drawPixel(pixels, matrixPos, &shadedColor);
            // drawPixel(pixels, matrixPos, &color);
        }
    }
}

std::pair<Point, Point> BarycentricRasterizer::findWindowingRectangle(
    const Polygon &polygon,
    const std::vector<DrawableVertex> &drawableVertices)
{
    //    const auto &firstVertex = vertices.at(polygon.cGetVertexIds(0).cGetVertexId() - 1);

    const auto &firstVertex = drawableVertices.at(polygon.cGetVertexIds(0).cGetVertexId() - 1);

    auto minX = (int)std::floor(firstVertex.CGetX());
    auto maxX = (int)std::ceil(firstVertex.CGetX());
    auto minY = (int)std::floor(firstVertex.CGetY());
    auto maxY = (int)std::ceil(firstVertex.CGetY());

    const auto size = polygon.cGetVertexIdsCount();
    for (int i = 0; i < size; ++i)
    {
        //        const auto &vertex = vertices.at(polygon.cGetVertexIds(i).cGetVertexId() - 1);

        const auto &vertex = drawableVertices.at(polygon.cGetVertexIds(i).cGetVertexId() - 1);

        if (vertex.CGetX() < minX)
            minX = (int)std::floor(vertex.CGetX());
        else if (vertex.CGetX() > maxX)
            maxX = (int)std::ceil(vertex.CGetX());
        if (vertex.CGetY() < minY)
            minY = (int)std::floor(vertex.CGetY());
        else if (vertex.CGetY() > maxY)
            maxY = (int)std::ceil(vertex.CGetY());
    }

    return {{minX, minY},
            {maxX, maxY}};
}

sf::Color BarycentricRasterizer::getShadedColor(
    const sf::Color &color,
    Polygon &polygon,
    const std::vector<Matrix<4, 1>> &vertices,
    const Matrix<4, 1> &lightDirection,
    ShadingModel shadingModel,
    const BaseLightingModel *lightingModel)
{
    sf::Color result;

    switch (shadingModel)
    {
    case Flat:
    {
        const auto invLightDir = lightDirection * -1;

        const auto normal = polygon.getNormal(vertices);
        const auto intensity = lightingModel->getLightIntensity(normal, invLightDir);

        result = sf::Color(color.r * intensity, color.g * intensity, color.b * intensity);

        break;
    }
    }

    return result;
}
