#pragma once

#include <cstring>
#include <SFML/Graphics.hpp>
#include <Scene.hpp>

class MainWindow
{
public:
    MainWindow(Point &_resolution);

    void switchVideoMode(const bool isEscape = false);
    void resize(const int width, const int height);

    void clear();

    void drawPixels();
    void drawModel(const Object &objInfo, const std::vector<Vertex> &viewportVertices);

    sf::RenderWindow &getWindow();
    const Point &cGetResolution() const;

private:
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

    sf::RenderWindow window;
    sf::Uint8 *pixels;
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
    const int x,
    const int y,
    const sf::Color *color,
    const int xSize)
{
    std::memcpy(pixels + (4 * (y * xSize + x)), color, 4);
}
