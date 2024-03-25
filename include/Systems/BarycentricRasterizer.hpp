#pragma once

#include <vector>
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
    // TODO: replace raw pointers with smart

    BarycentricRasterizer(
        const BaseLightingModel *_lightingModel,
        const BaseLightSource *_lightSource,
        ShadingModel _shadingModel,
        const Point &_resolution,
        void (*_drawPixel)(sf::Uint8 *, const int, const sf::Color *),
        sf::Uint8 *_pixels,
        omp_lock_t *_pixelLocks,
        double *_depthBuffer);

    void resize(
        sf::Uint8 *newPixels,
        double *newDepthBuffer,
        omp_lock_t *newPixelLocks);

    void rasterize(
        Triangle &polygon,
        const Vector<4> &sightDir,
        const Object &object);

    static std::pair<Point, Point> findWindowingRectangle(
        const Triangle &polygon,
        const std::vector<Vector<4>> &drawableVertices);

    static double edgeFunction(
        const Vector<4> &a,
        const Vector<4> &b,
        const Vector<4> &c);

    static void calcBarycentric(
        const Point &p,
        const Vector<4> &a,
        const Vector<2> &v0,
        const Vector<2> &v1,
        double invDen,
        double &b0, double &b1, double &b2);

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

    sf::Color getShadedColor(
        Triangle &polygon,
        const Vector<4> &cameraPosition,
        const double b0, const double b1, const double b2,
        const double invAW, const double invBW, const double invCW,
        const Object &object,
        const Vector<4> &lightDirection);

    Vector<4> getMapId(
        const Triangle &polygon,
        const Object &object,
        const double b0, const double b1, const double b2,
        const double invAW, const double invBW, const double invCW);

    Vector<4> getMapValue(
        const std::shared_ptr<const Texture> map,
        const Vector<4> &mapId);

    Vector<4> getNormalByShading(
        Triangle &polygon,
        const Object &object,
        const double b0, const double b1, const double b2);
};

inline double BarycentricRasterizer::edgeFunction(
    const Vector<4> &a,
    const Vector<4> &b,
    const Vector<4> &c)
{
    return (c.cGetX() - a.cGetX()) * (b.cGetY() - a.cGetY()) -
           (c.cGetY() - a.cGetY()) * (b.cGetX() - a.cGetX());
}

inline void BarycentricRasterizer::calcBarycentric(
    const Point &p,
    const Vector<4> &a,
    const Vector<2> &v0,
    const Vector<2> &v1,
    const double invDen,
    double &b0, double &b1, double &b2)
{
    const auto v2X = p.cGetX() - a.cGetX();
    const auto v2Y = p.cGetY() - a.cGetY();

    b1 = (v2X * v1.cGetY() - v1.cGetX() * v2Y) * invDen;
    b2 = (v0.cGetX() * v2Y - v2X * v0.cGetY()) * invDen;
    b0 = 1 - b2 - b1;
}
