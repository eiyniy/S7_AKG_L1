#pragma once

#include <vector>
#include <DrawableVertex.hpp>
#include <Triangle.hpp>
#include <SFML/Config.hpp>
#include <SFML/Graphics/Color.hpp>
#include <Point.hpp>
#include <BaseLightingModel.hpp>
#include <BaseLightSource.hpp>
#include <Timer.hpp>
#include <omp.h>
#include <Object.hpp>

class BarycentricRasterizer
{
public:
    BarycentricRasterizer(
        const BaseLightingModel *_lightingModel,
        const BaseLightSource *_lightSource,
        ShadingModel _shadingModel,
        const Point &_resolution,
        void (*_drawPixel)(sf::Uint8 *, const int, const sf::Color *),
        sf::Uint8 *_pixels,
        omp_lock_t *_pixelLocks,
        double *_depthBuffer);

    void rasterize(
        Triangle &polygon,
        const Matrix<4, 1> &sightDir,
        const Object &object);

private:
    const BaseLightingModel *lightingModel;
    const BaseLightSource *lightSource;
    const ShadingModel shadingModel;

    const Point &resolution;

    void (*drawPixel)(
        sf::Uint8 *pixels,
        const int pos,
        const sf::Color *color);

    sf::Uint8 *pixels;
    omp_lock_t *pixelLocks;
    double *depthBuffer;

    static std::pair<Point, Point> findWindowingRectangle(
        const Triangle &polygon,
        const std::vector<DrawableVertex> &drawableVertices);

    static sf::Color getShadedColor(
        Triangle &polygon,
        const Matrix<4, 1> &cameraPosition,
        const double &w0, const double &w1, const double &w2,
        const double invZ0, const double invZ1, const double invZ2,
        const Object &object,
        const Matrix<4, 1> &lightDirection,
        ShadingModel shadingModel,
        const BaseLightingModel *lightingModel);

    static double edgeFunction(
        const DrawableVertex &a,
        const DrawableVertex &b,
        const DrawableVertex &c);

    static void calcBarycentric(
        const Matrix<4, 1> &p,
        const Matrix<4, 1> &a,
        const Matrix<4, 1> &b,
        const Matrix<4, 1> &c,
        double &w0, double &w1, double &w2);

    static void calcBarycentric(
        const Matrix<4, 1> &p,
        const Matrix<4, 1> &a,
        const Matrix<4, 1> &v0,
        const Matrix<4, 1> &v1,
        double invDen,
        double &w0, double &w1, double &w2);

    static void calcBarycentric(
        const DrawableVertex &p,
        const DrawableVertex &a,
        const DrawableVertex &v0,
        const DrawableVertex &v1,
        double invDen,
        double &w0, double &w1, double &w2);
};

inline double BarycentricRasterizer::edgeFunction(
    const DrawableVertex &a,
    const DrawableVertex &b,
    const DrawableVertex &c)
{
    return (c.CGetX() - a.CGetX()) * (b.CGetY() - a.CGetY()) -
           (c.CGetY() - a.CGetY()) * (b.CGetX() - a.CGetX());
}

inline void BarycentricRasterizer::calcBarycentric(
    const Matrix<4, 1> &p,
    const Matrix<4, 1> &a,
    const Matrix<4, 1> &b,
    const Matrix<4, 1> &c,
    double &w0, double &w1, double &w2)
{
    const auto v0 = b - a;
    const auto v1 = c - a;
    const auto v2 = p - a;

    const double d00 = v0.scalarMultiply(v0);
    const double d01 = v0.scalarMultiply(v1);
    const double d11 = v1.scalarMultiply(v1);
    const double d20 = v2.scalarMultiply(v0);
    const double d21 = v2.scalarMultiply(v1);

    const double denom = d00 * d11 - d01 * d01;

    w0 = (d11 * d20 - d01 * d21) / denom;
    w1 = (d00 * d21 - d01 * d20) / denom;
    w2 = 1.f - w0 - w1;
}

inline void BarycentricRasterizer::calcBarycentric(
    const Matrix<4, 1> &p,
    const Matrix<4, 1> &a,
    const Matrix<4, 1> &v0,
    const Matrix<4, 1> &v1,
    const double invDen,
    double &w0, double &w1, double &w2)
{
    const auto v2 = p - a;

    w0 = (v2.cGetX() * v1.cGetY() - v1.cGetX() * v2.cGetY()) * invDen;
    w1 = (v0.cGetX() * v2.cGetY() - v2.cGetX() * v0.cGetY()) * invDen;
    w2 = 1 - w0 - w1;
}

inline void BarycentricRasterizer::calcBarycentric(
    const DrawableVertex &p,
    const DrawableVertex &a,
    const DrawableVertex &v0,
    const DrawableVertex &v1,
    const double invDen,
    double &w0, double &w1, double &w2)
{
    const DrawableVertex v2{p.CGetX() - a.CGetX(), p.CGetY() - a.CGetY(), 0};

    w1 = (v2.CGetX() * v1.CGetY() - v1.CGetX() * v2.CGetY()) * invDen;
    w2 = (v0.CGetX() * v2.CGetY() - v2.CGetX() * v0.CGetY()) * invDen;
    w0 = 1 - w2 - w1;
}
