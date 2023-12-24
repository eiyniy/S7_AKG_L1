#include <BarycentricRasterizer.hpp>
#include <Converter.hpp>
#include <iostream>
#include <PhongModel.hpp>

BarycentricRasterizer::BarycentricRasterizer(
    const BaseLightingModel *_lightingModel,
    const BaseLightSource *_lightSource,
    const ShadingModel _shadingModel,
    const Point &_resolution,
    void (*_drawPixel)(sf::Uint8 *pixels, const int, const sf::Color *),
    sf::Uint8 *_pixels,
    omp_lock_t *_pixelLocks,
    double *_depthBuffer)
    : lightingModel(_lightingModel),
      lightSource(_lightSource),
      shadingModel(_shadingModel),
      resolution(_resolution),
      drawPixel(_drawPixel),
      pixels(_pixels),
      pixelLocks(_pixelLocks),
      depthBuffer(_depthBuffer) {}

void BarycentricRasterizer::rasterize(
    Triangle &polygon,
    const Matrix<4, 1> &cameraPosition,
    const Object &object)
{
    const auto &drawableVertices = object.cGetDrawable();
    const auto &vertices = object.cGetVertices();

    const auto vId0 = polygon.cGetVertexIds(0).cGetVertexId();
    const auto vId1 = polygon.cGetVertexIds(1).cGetVertexId();
    const auto vId2 = polygon.cGetVertexIds(2).cGetVertexId();

    const auto &aDrawable = drawableVertices.at(vId0 - 1);
    const auto &bDrawable = drawableVertices.at(vId1 - 1);
    const auto &cDrawable = drawableVertices.at(vId2 - 1);

    const auto &a = vertices.at(vId0 - 1);
    const auto &b = vertices.at(vId1 - 1);
    const auto &c = vertices.at(vId2 - 1);

    const auto area = edgeFunction(aDrawable, bDrawable, cDrawable);
    if (area == 0)
        return;
    const auto invArea = 1 / area;

    // std::cout << aDrawable.CGetZ() << " " << bDrawable.CGetZ() << " " << cDrawable.CGetZ() << std::endl;

    const auto invAZ = 1 / aDrawable.CGetZ();
    const auto invBZ = 1 / bDrawable.CGetZ();
    const auto invCZ = 1 / cDrawable.CGetZ();

    const auto windowingRectangle = findWindowingRectangle(polygon, drawableVertices);

    // const int minX = windowingRectangle.first.cGetX();
    // const int minY = windowingRectangle.first.cGetY();
    // const int maxX = windowingRectangle.second.cGetX();
    // const int maxY = windowingRectangle.second.cGetY();

    const int minX = std::max(windowingRectangle.first.cGetX(), 0);
    const int minY = std::max(windowingRectangle.first.cGetY(), 0);
    const int maxX = std::min(windowingRectangle.second.cGetX(), resolution.cGetX() - 1);
    const int maxY = std::min(windowingRectangle.second.cGetY(), resolution.cGetY() - 1);

    //    /*
    //    const Matrix<4, 1> minMinVertex{(double) minX, (double) minY, 0};
    //    const Matrix<4, 1> maxMinVertex{(double) maxX, (double) minY, 0};
    //    const Matrix<4, 1> minMaxVertex{(double) minX, (double) maxY, 0};

    const DrawableVertex minMinVertex{minX, minY, 0};
    const DrawableVertex maxMinVertex{maxX, minY, 0};
    const DrawableVertex minMaxVertex{minX, maxY, 0};

    //    const auto v0 = bMatrix - aMatrix;
    //    const auto v1 = cMatrix - aMatrix;

    const DrawableVertex v0{bDrawable.CGetX() - aDrawable.CGetX(), bDrawable.CGetY() - aDrawable.CGetY(), 0};
    const DrawableVertex v1{cDrawable.CGetX() - aDrawable.CGetX(), cDrawable.CGetY() - aDrawable.CGetY(), 0};

    const auto invDen = 1 / (v0.CGetX() * v1.CGetY() - v1.CGetX() * v0.CGetY());

    double w0, w1, w2;

    w0 = edgeFunction(bDrawable, cDrawable, minMinVertex) * invArea;
    w1 = edgeFunction(cDrawable, aDrawable, minMinVertex) * invArea;
    w2 = 1 - w0 - w1;

    // calcBarycentric(minMinVertex, aDrawable, v0, v1, invDen, w0, w1, w2);
    //    calcBarycentric(minMinVertex, aMatrix, v0, v1, invDen, w0, w1, w2);
    //    calcBarycentric(minMinVertex, aMatrix, bMatrix, cMatrix, w0, w1, w2);

    double w0XLast, w1XLast, w2XLast;

    w0XLast = edgeFunction(bDrawable, cDrawable, maxMinVertex) * invArea;
    w1XLast = edgeFunction(cDrawable, aDrawable, maxMinVertex) * invArea;
    w2XLast = 1 - w0XLast - w1XLast;

    // calcBarycentric(maxMinVertex, aDrawable, v0, v1, invDen, w0XLast, w1XLast, w2XLast);
    //    calcBarycentric(maxMinVertex, aMatrix, v0, v1, invDen, w0XLast, w1XLast, w2XLast);
    //    calcBarycentric(maxMinVertex, aMatrix, bMatrix, cMatrix, w0XLast, w1XLast, w2XLast);

    double w0YLast, w1YLast, w2YLast;

    w0YLast = edgeFunction(bDrawable, cDrawable, minMaxVertex) * invArea;
    w1YLast = edgeFunction(cDrawable, aDrawable, minMaxVertex) * invArea;
    w2YLast = 1 - w0YLast - w1YLast;

    // calcBarycentric(minMaxVertex, aDrawable, v0, v1, invDen, w0YLast, w1YLast, w2YLast);
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

    sf::Color shadedColor;

    if (shadingModel == ShadingModel::Flat)
    {
        auto sightDir = polygon.getCenter(vertices) - cameraPosition;
        sightDir.normalize();

        shadedColor = getShadedColor(
            polygon,
            sightDir,
            w0, w1, w2,
            invAZ, invBZ, invCZ,
            object,
            lightSource->getLightDirection(polygon.getCenter(vertices)),
            shadingModel,
            lightingModel);
    }

    //    */
    for (int i = minY; i <= maxY; ++i, w0 += w0YStep, w1 += w1YStep, w2 += w2YStep)
    {
        for (int j = minX; j <= maxX; ++j, w0 += w0XStep, w1 += w1XStep, w2 += w2XStep)
        {
            // calcBarycentric({j, i, 0}, aDrawable, v0, v1, invDen, w0, w1, w2);

            if (w0 < 0 || w1 < 0 || w2 < 0)
                continue;

            // const auto z = aDrawable.CGetZ() * w0 + bDrawable.CGetZ() * w1 + cDrawable.CGetZ() * w2;
            const auto z = 1 / (invAZ * w0 + invBZ * w1 + invCZ * w2);
            const auto matrixPos = i * resolution.cGetX() + j;

            auto oldZ = depthBuffer[matrixPos];
            if (z < oldZ)
            {
                omp_set_lock(&pixelLocks[matrixPos]);

                oldZ = depthBuffer[matrixPos];
                if (z < oldZ)
                {
                    depthBuffer[matrixPos] = z;

                    if (shadingModel == ShadingModel::Phong)
                    {
                        const auto p = a * w0 + b * w1 + c * w2;
                        auto sightDir = p - cameraPosition;
                        sightDir.normalize();

                        shadedColor = getShadedColor(
                            polygon,
                            sightDir,
                            w0, w1, w2,
                            invAZ, invBZ, invCZ,
                            object,
                            lightSource->getLightDirection(p),
                            shadingModel,
                            lightingModel);
                    }

                    // drawPixel(pixels, matrixPos, &polygon.color);
                    drawPixel(pixels, matrixPos, &shadedColor);
                    // drawPixel(pixels, matrixPos, &color);
                }

                omp_unset_lock(&pixelLocks[matrixPos]);
            }
        }
    }
}

std::pair<Point, Point> BarycentricRasterizer::findWindowingRectangle(
    const Triangle &polygon,
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
    Triangle &polygon,
    const Matrix<4, 1> &sightDirection,
    const double &w0, const double &w1, const double &w2,
    const double invZ0, const double invZ1, const double invZ2,
    const Object &object,
    const Matrix<4, 1> &lightDirection,
    ShadingModel shadingModel,
    const BaseLightingModel *lightingModel)
{
    const auto invLightDir = lightDirection * -1;

    sf::Color result;
    Matrix<4, 1> normal;
    sf::Color diffuseColor;
    Matrix<4, 1> mrao;
    sf::Color emissiveColor;

    const auto tId0 = polygon.cGetVertexIds(0).cGetTextureVertexId();
    const auto tId1 = polygon.cGetVertexIds(1).cGetTextureVertexId();
    const auto tId2 = polygon.cGetVertexIds(2).cGetTextureVertexId();

    if (!tId0.has_value() || !tId2.has_value() || !tId2.has_value())
        throw std::runtime_error("Can not get diffuseMap");

    const auto &vertices = object.cGetVertices();
    const auto &tVertices = object.cGetTVertices();

    const auto &aMapId = tVertices.at(*tId0 - 1);
    const auto &bMapId = tVertices.at(*tId1 - 1);
    const auto &cMapId = tVertices.at(*tId2 - 1);

    const auto mapIdNominator = aMapId * invZ0 * w0 + bMapId * invZ1 * w1 + cMapId * invZ2 * w2;
    const auto mapIdDenominator = invZ0 * w0 + invZ1 * w1 + invZ2 * w2;
    const auto mapId = mapIdNominator / mapIdDenominator;

    const auto &diffuseMap = object.cGetDiffuseMap();
    if (diffuseMap.has_value())
    {
        const auto width = diffuseMap->cGetWidth() - 1;
        const auto height = diffuseMap->cGetHeight() - 1;

        const Point normMapId{
            (int)(mapId.cGetX() * width),
            (int)(height - mapId.cGetY() * height)};

        diffuseColor = diffuseMap->cGetData().at(normMapId.cGetX() + (width + 1) * normMapId.cGetY());
    }
    else
        diffuseColor = sf::Color::White;

    const auto &normalMap = object.cGetNormalMap();
    if (normalMap.has_value())
    {
        const auto width = normalMap->cGetWidth() - 1;
        const auto height = normalMap->cGetHeight() - 1;

        const Point normMapId{
            (int)(mapId.cGetX() * width),
            (int)(height - mapId.cGetY() * height)};

        normal = normalMap->cGetData().at(normMapId.cGetX() + (width + 1) * normMapId.cGetY());
    }
    else
    {
        switch (shadingModel)
        {
        case Flat:
        {
            normal = polygon.getNormal(vertices);

            break;
        }
        case Phong:
        {
            const auto nId0 = polygon.cGetVertexIds(0).cGetNormalVertexId();
            const auto nId1 = polygon.cGetVertexIds(1).cGetNormalVertexId();
            const auto nId2 = polygon.cGetVertexIds(2).cGetNormalVertexId();

            if (!nId0.has_value() || !nId2.has_value() || !nId2.has_value())
                throw std::runtime_error("Can not get normal");

            const auto &nVertices = object.cGetNVertices();

            const auto &aNormal = nVertices.at(*nId0 - 1);
            const auto &bNormal = nVertices.at(*nId1 - 1);
            const auto &cNormal = nVertices.at(*nId2 - 1);

            normal = aNormal * w0 + bNormal * w1 + cNormal * w2;
            normal.normalize();

            break;
        }
        }
    }

    const auto &mraoMap = object.cGetMRAOMap();
    if (mraoMap.has_value())
    {
        const auto width = mraoMap->cGetWidth() - 1;
        const auto height = mraoMap->cGetHeight() - 1;

        const Point normMapId{
            (int)(mapId.cGetX() * width),
            (int)(height - mapId.cGetY() * height)};

        mrao = mraoMap->cGetData().at(normMapId.cGetX() + (width + 1) * normMapId.cGetY());
    }
    else
    {
        const auto phongModel = ((PhongModel *)lightingModel);
        mrao = Matrix<4, 1>(1, 1, phongModel != nullptr ? phongModel->cGetAmbientCoeff() : 1);
    }

    const auto &emissiveMap = object.cGetEmissiveMap();
    if (emissiveMap.has_value())
    {
        const auto width = emissiveMap->cGetWidth() - 1;
        const auto height = emissiveMap->cGetHeight() - 1;

        const Point normMapId{
            (int)(mapId.cGetX() * width),
            (int)(height - mapId.cGetY() * height)};

        emissiveColor = emissiveMap->cGetData().at(normMapId.cGetX() + (width + 1) * normMapId.cGetY());
    }
    else
        emissiveColor = sf::Color(0, 0, 0);

    const auto color = lightingModel->getLightIntensity(normal, diffuseColor, invLightDir, sightDirection, mrao);

    result = sf::Color(
        std::min(color.r + emissiveColor.r, 255),
        std::min(color.g + emissiveColor.g, 255),
        std::min(color.b + emissiveColor.b, 255));

    return result;
}
