#pragma once

#include <cstring>
#include <SFML/Graphics.hpp>
#include <Scene.hpp>
#include <mutex>
#include <memory>
#include <CSClipper.hpp>

class MainWindow {
public:
    MainWindow(Point &_resolution);

    void switchVideoMode(const bool isEscape = false);

    void resize(const int width, const int height);

    void clear();

    void drawPixels();

    void drawModel(Object &objInfo, std::vector<Vertex> viewportVertices);

    sf::RenderWindow &getWindow();

    const Point &cGetResolution() const;

private:
    void drawPolygon(
            const Polygon &polygon,
            std::vector<Vertex> &drawableVertices,
            const sf::Color *color);

    void drawLineBr(
            const Point &p1,
            const Point &p2,
            const sf::Color *color);

    void drawPixel(
            const int x,
            const int y,
            const sf::Color *color,
            const int xSize);

    bool isFullscreen;

    Point &resolution;
    Point lastResolution;

    sf::Uint8 *pixels;
    std::mutex pixelsMutex;

    sf::RenderWindow window;
    sf::Texture bufferTexture;
    sf::Sprite bufferSprite;

    std::unique_ptr<CSClipper> clipper;
};

inline sf::RenderWindow &MainWindow::getWindow() {
    return window;
}

inline const Point &MainWindow::cGetResolution() const {
    return resolution;
}

inline void MainWindow::drawPixel(
        const int x,
        const int y,
        const sf::Color *color,
        const int xSize) {
    // std::unique_lock<std::mutex> pixelsLock(pixelsMutex);
    std::memcpy(pixels + (4 * (y * xSize + x)), color, 4);
}
