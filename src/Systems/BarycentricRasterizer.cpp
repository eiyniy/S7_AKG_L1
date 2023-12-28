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

void BarycentricRasterizer::resize(sf::Uint8 *newPixels, double *newDepthBuffer, omp_lock_t *newPixelLocks)
{
    pixels = newPixels;
    depthBuffer = newDepthBuffer;
    pixelLocks = newPixelLocks;
}

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

    const auto invAZ = 1 / aDrawable.cGetZ();
    const auto invBZ = 1 / bDrawable.cGetZ();
    const auto invCZ = 1 / cDrawable.cGetZ();

    const auto invAW = 1 / aDrawable.cGetW();
    const auto invBW = 1 / bDrawable.cGetW();
    const auto invCW = 1 / cDrawable.cGetW();

    const auto windowingRectangle = findWindowingRectangle(polygon, drawableVertices);

    const int minX = std::max(windowingRectangle.first.cGetX(), 0);
    const int minY = std::max(windowingRectangle.first.cGetY(), 0);
    const int maxX = std::min(windowingRectangle.second.cGetX(), resolution.cGetX() - 1);
    const int maxY = std::min(windowingRectangle.second.cGetY(), resolution.cGetY() - 1);

    const Matrix<4, 1> minMinVertex{minX, minY, 0};
    const Matrix<4, 1> maxMinVertex{maxX, minY, 0};
    const Matrix<4, 1> minMaxVertex{minX, maxY, 0};

    const Matrix<4, 1> v0{bDrawable.cGetX() - aDrawable.cGetX(), bDrawable.cGetY() - aDrawable.cGetY(), 0, 0};
    const Matrix<4, 1> v1{cDrawable.cGetX() - aDrawable.cGetX(), cDrawable.cGetY() - aDrawable.cGetY(), 0, 0};

    const auto invDen = 1 / (v0.cGetX() * v1.cGetY() - v1.cGetX() * v0.cGetY());

    double b0, b1, b2;
    calcBarycentric(minMinVertex, aDrawable, v0, v1, invDen, b0, b1, b2);

    double b0XLast, b1XLast, b2XLast;
    calcBarycentric(maxMinVertex, aDrawable, v0, v1, invDen, b0XLast, b1XLast, b2XLast);

    double b0YLast, b1YLast, b2YLast;
    calcBarycentric(minMaxVertex, aDrawable, v0, v1, invDen, b0YLast, b1YLast, b2YLast);

    const auto xInterval = maxX - minX;
    const auto yInterval = maxY - minY;

    const auto b0XStep = (b0XLast - b0) / xInterval;
    const auto b1XStep = (b1XLast - b1) / xInterval;
    const auto b2XStep = (b2XLast - b2) / xInterval;

    const auto b0RowStep = b0XStep * (xInterval + 1);
    const auto b1RowStep = b1XStep * (xInterval + 1);
    const auto b2RowStep = b2XStep * (xInterval + 1);

    const auto b0YStep = (b0YLast - b0) / yInterval - b0RowStep;
    const auto b1YStep = (b1YLast - b1) / yInterval - b1RowStep;
    const auto b2YStep = (b2YLast - b2) / yInterval - b2RowStep;

    // std::cout << "b0: " << b0  << ", b1: " << b1 << ", b2: " << b2 << std::endl;
    // std::cout << "b0XStep: " << b0XStep << ", b1XStep: " << b1XStep << ", b2XStep: " << b2XStep << std::endl;
    // std::cout << "b0YStep: " << b0YStep << ", b1YStep: " << b1YStep << ", b2YStep: " << b2YStep << std::endl;
    // std::cout << std::endl;

    sf::Color shadedColor;

    if (shadingModel == ShadingModel::Flat)
    {
        auto sightDir = polygon.getCenter(vertices) - cameraPosition;
        sightDir.normalize();

        shadedColor = getShadedColor(
            polygon,
            sightDir,
            b0, b1, b2,
            invAW, invBW, invCW,
            object,
            lightSource->getLightDirection(polygon.getCenter(vertices)));
    }

    for (int i = minY; i <= maxY; ++i, b0 += b0YStep, b1 += b1YStep, b2 += b2YStep)
    {
        for (int j = minX; j <= maxX; ++j, b0 += b0XStep, b1 += b1XStep, b2 += b2XStep)
        {
            if (b0 < 0 || b1 < 0 || b2 < 0)
                continue;

            const auto z = 1 / (invAZ * b0 + invBZ * b1 + invCZ * b2);
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
                        const auto p = a * b0 + b * b1 + c * b2;
                        auto sightDir = p - cameraPosition;
                        sightDir.normalize();

                        shadedColor = getShadedColor(
                            polygon,
                            sightDir,
                            b0, b1, b2,
                            invAW, invBW, invCW,
                            object,
                            lightSource->getLightDirection(p));
                    }

                    drawPixel(pixels, matrixPos, &shadedColor);
                }

                omp_unset_lock(&pixelLocks[matrixPos]);
            }
        }
    }
}

std::pair<Point, Point> BarycentricRasterizer::findWindowingRectangle(
    const Triangle &polygon,
    const std::vector<Matrix<4, 1>> &drawableVertices)
{
    const auto &firstVertex = drawableVertices.at(polygon.cGetVertexIds(0).cGetVertexId() - 1);

    auto minX = (int)std::floor(firstVertex.cGetX());
    auto maxX = (int)std::ceil(firstVertex.cGetX());
    auto minY = (int)std::floor(firstVertex.cGetY());
    auto maxY = (int)std::ceil(firstVertex.cGetY());

    const auto size = polygon.cGetVertexIdsCount();
    for (int i = 0; i < size; ++i)
    {
        const auto &vertex = drawableVertices.at(polygon.cGetVertexIds(i).cGetVertexId() - 1);

        if (vertex.cGetX() < minX)
            minX = (int)std::floor(vertex.cGetX());
        else if (vertex.cGetX() > maxX)
            maxX = (int)std::ceil(vertex.cGetX());
        if (vertex.cGetY() < minY)
            minY = (int)std::floor(vertex.cGetY());
        else if (vertex.cGetY() > maxY)
            maxY = (int)std::ceil(vertex.cGetY());
    }

    return {{minX, minY},
            {maxX, maxY}};
}

sf::Color BarycentricRasterizer::getShadedColor(
    Triangle &polygon,
    const Matrix<4, 1> &sightDirection,
    const double b0, const double b1, const double b2,
    const double invAW, const double invBW, const double invCW,
    const Object &object,
    const Matrix<4, 1> &lightDirection)
{
    const auto invLightDir = lightDirection * -1;

    sf::Color result;
    Matrix<4, 1> mapId;
    Matrix<4, 1> normal;
    Matrix<4, 1> diffuse;
    Matrix<4, 1> mrao;
    Matrix<4, 1> emissive;

    const auto &diffuseMap = object.cGetDiffuseMap();
    const auto &normalMap = object.cGetNormalMap();
    const auto &mraoMap = object.cGetMRAOMap();
    const auto &emissiveMap = object.cGetEmissiveMap();

    if (diffuseMap.has_value() || normalMap.has_value() || mraoMap.has_value() || emissiveMap.has_value())
        mapId = getMapId(polygon, object, b0, b1, b2, invAW, invBW, invCW);

    if (diffuseMap.has_value())
        diffuse = getMapValue(*diffuseMap, mapId);
    else
        diffuse = Converter::colorToMatrix(sf::Color::White);

    if (normalMap.has_value())
        normal = getMapValue(*normalMap, mapId);
    else
        normal = getNormalByShading(polygon, object, b0, b1, b2);

    if (mraoMap.has_value())
        mrao = getMapValue(*mraoMap, mapId);
    else
    {
        const auto phongModel = ((PhongModel *)lightingModel);
        mrao = Matrix<4, 1>(1, 1, phongModel != nullptr ? phongModel->cGetAmbientCoeff() : 1);
    }

    if (emissiveMap.has_value())
        emissive = getMapValue(*emissiveMap, mapId);
    else
        emissive = Converter::colorToMatrix(sf::Color::Black);

    const auto color = lightingModel->getLightIntensity(normal, diffuse, invLightDir, sightDirection, mrao);

    result = sf::Color(
        std::min(color.r + emissive.cGetX(), 255.0),
        std::min(color.g + emissive.cGetY(), 255.0),
        std::min(color.b + emissive.cGetZ(), 255.0));

    return result;
}

Matrix<4, 1> BarycentricRasterizer::getMapId(
    const Triangle &polygon,
    const Object &object,
    const double b0, const double b1, const double b2,
    const double invAW, const double invBW, const double invCW)
{
    const auto tId0 = polygon.cGetVertexIds(0).cGetTextureVertexId();
    const auto tId1 = polygon.cGetVertexIds(1).cGetTextureVertexId();
    const auto tId2 = polygon.cGetVertexIds(2).cGetTextureVertexId();

    if (!tId0.has_value() || !tId1.has_value() || !tId2.has_value())
        throw std::runtime_error("Can not get texture coordinates");

    const auto &tVertices = object.cGetTVertices();

    const auto &aMapId = tVertices.at(*tId0 - 1);
    const auto &bMapId = tVertices.at(*tId1 - 1);
    const auto &cMapId = tVertices.at(*tId2 - 1);

    const auto mapIdNominator = aMapId * invAW * b0 + bMapId * invBW * b1 + cMapId * invCW * b2;
    const auto mapIdDenominator = invAW * b0 + invBW * b1 + invCW * b2;
    return mapIdNominator / mapIdDenominator;
}

Matrix<4, 1> BarycentricRasterizer::getMapValue(
    const Texture &map,
    const Matrix<4, 1> &mapId)
{
    const auto width = map.cGetWidth();
    const auto height = map.cGetHeight();

    const Point normMapId{
        (int)(mapId.cGetX() * (width - 1)),
        (int)(height - mapId.cGetY() * (height - 1))};

    return map.cGetData().at(normMapId.cGetX() + width * normMapId.cGetY());
}

Matrix<4, 1> BarycentricRasterizer::getNormalByShading(
    Triangle &polygon,
    const Object &object,
    const double b0, const double b1, const double b2)
{
    Matrix<4, 1> normal;

    switch (shadingModel)
    {
    case Flat:
    {
        const auto &vertices = object.cGetVertices();
        normal = polygon.getNormal(vertices);

        break;
    }
    case Phong:
    {
        const auto nId0 = polygon.cGetVertexIds(0).cGetNormalVertexId();
        const auto nId1 = polygon.cGetVertexIds(1).cGetNormalVertexId();
        const auto nId2 = polygon.cGetVertexIds(2).cGetNormalVertexId();

        if (!nId0.has_value() || !nId1.has_value() || !nId2.has_value())
            throw std::runtime_error("Can not get normal");

        const auto &nVertices = object.cGetNVertices();

        const auto &aNormal = nVertices.at(*nId0 - 1);
        const auto &bNormal = nVertices.at(*nId1 - 1);
        const auto &cNormal = nVertices.at(*nId2 - 1);

        normal = aNormal * b0 + bNormal * b1 + cNormal * b2;
        normal.normalize();

        break;
    }
    }

    return normal;
}
