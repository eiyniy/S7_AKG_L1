#pragma once

#include <cstring>
#include <SFML/Graphics.hpp>
#include <Scene.hpp>
#include <mutex>
#include <memory>
#include <BaseLightingModel.hpp>
#include <BaseLightSource.hpp>
#include <BarycentricRasterizer.hpp>
#include <Types.hpp>
#include <omp.h>
#include <Sculptor.hpp>

class MainWindow
{
public:
    MainWindow(
        Point &_resolution,
        const BaseLightingModel *_lightingModel,
        const BaseLightSource *_lightSource,
        const ShadingModel _shadingModel);

    void switchVideoMode(bool isEscape = false);

    void resize(int width, int height);

    void clear();

    void drawPixels();

    void drawModel(
        Object &objInfo,
        const Vector<4> &cameraPosition);

    void drawSculptor(
        Sculptor &sculptor,
        const Point &mousePos);

    sf::RenderWindow &getWindow();

    const Point &cGetResolution() const;

    int getPointedTriangleId(
        const Object *objInfo,
        const Point mousePos) const;

private:
    static bool isPolygonFrontOriented(
        Triangle &polygon,
        const std::vector<Vector<4>> &vertices,
        const Vector<4> &sightDir);

    void drawPolygon(
        Triangle &polygon,
        const Vector<4> &cameraPosition,
        const Object &object);

    void drawLineBr(
        const Point &p1,
        const Point &p2,
        const sf::Color *color);

    static void drawPixel(
        sf::Uint8 *pixels,
        int pos,
        const sf::Color *color);

    bool isFullscreen;

    Point &resolution;
    Point lastResolution;

    sf::Uint8 *pixels;
    omp_lock_t *pixelLocks;

    double *depthBuffer;

    BarycentricRasterizer rasterizer;

    sf::RenderWindow window;
    sf::Texture bufferTexture;
    sf::Sprite bufferSprite;
};

inline sf::RenderWindow &MainWindow::getWindow()
{
    return window;
}

inline const Point &MainWindow::cGetResolution() const
{
    return resolution;
}

inline void MainWindow::drawPixel(
    sf::Uint8 *pixels,
    const int pos,
    const sf::Color *color)
{
    static const auto colorSize = (int)sizeof(sf::Color);
    std::memcpy(pixels + (colorSize * pos), color, colorSize);
}

inline bool MainWindow::isPolygonFrontOriented(
    Triangle &polygon,
    const std::vector<Vector<4>> &vertices,
    const Vector<4> &sightDir)
{
    return polygon.getFlatNormal(vertices).scalarMultiply(sightDir) < 0;
}
