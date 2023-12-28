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

    sf::Color getShadedColor(
        Triangle &polygon,
        const Matrix<4, 1> &cameraPosition,
        const double b0, const double b1, const double b2,
        const double invAW, const double invBW, const double invCW,
        const Object &object,
        const Matrix<4, 1> &lightDirection);

    Matrix<4, 1> getMapId(
        const Triangle &polygon,
        const Object &object,
        const double b0, const double b1, const double b2,
        const double invAW, const double invBW, const double invCW);

    Matrix<4, 1> getMapValue(
        const Texture &map,
        const Matrix<4, 1> &mapId);

    Matrix<4, 1> getNormalByShading(
        Triangle &polygon,
        const Object &object,
        const double b0, const double b1, const double b2
    );

    static std::pair<Point, Point> findWindowingRectangle(
        const Triangle &polygon,
        const std::vector<Matrix<4, 1>> &drawableVertices);

    static double edgeFunction(
        const Matrix<4, 1> &a,
        const Matrix<4, 1> &b,
        const Matrix<4, 1> &c);

    static void calcBarycentric(
        const Matrix<4, 1> &p,
        const Matrix<4, 1> &a,
        const Matrix<4, 1> &v0,
        const Matrix<4, 1> &v1,
        double invDen,
        double &b0, double &b1, double &b2);
};

inline double BarycentricRasterizer::edgeFunction(
    const Matrix<4, 1> &a,
    const Matrix<4, 1> &b,
    const Matrix<4, 1> &c)
{
    return (c.cGetX() - a.cGetX()) * (b.cGetY() - a.cGetY()) -
           (c.cGetY() - a.cGetY()) * (b.cGetX() - a.cGetX());
}

inline void BarycentricRasterizer::calcBarycentric(
    const Matrix<4, 1> &p,
    const Matrix<4, 1> &a,
    const Matrix<4, 1> &v0,
    const Matrix<4, 1> &v1,
    const double invDen,
    double &b0, double &b1, double &b2)
{
    const Matrix<4, 1> v2{p.cGetX() - a.cGetX(), p.cGetY() - a.cGetY(), 0, 0};

    b1 = (v2.cGetX() * v1.cGetY() - v1.cGetX() * v2.cGetY()) * invDen;
    b2 = (v0.cGetX() * v2.cGetY() - v2.cGetX() * v0.cGetY()) * invDen;
    b0 = 1 - b2 - b1;
}
