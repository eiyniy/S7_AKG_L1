#pragma once

#include <cstring>
#include <SFML/Graphics.hpp>
#include <Scene.hpp>

class MainWindow
{
public:
    MainWindow(Scene &p_scene);

    void startLoop();
    void drawAllModels();

private:
    void drawLineBR_1(
        sf::Uint8 *pixels,
        const Vertex &v1,
        const Vertex &v2,
        const sf::Color *color);
    void drawLineDDA(
        sf::Uint8 *pixels,
        const Vertex &v1,
        const Vertex &v2,
        const sf::Color *color);

    void drawModel(const ObjInfo &objInfo, const std::vector<Vertex> &viewportVertices);
    void drawPixel(
        const int x,
        const int y,
        const sf::Color *color,
        const int xSize);

    bool isCameraMoving,
        isCameraRotating,
        isCameraRotatingAround,
        isObjectMoving,
        isCentering;

    Scene &scene;

    sf::RenderWindow window;
    sf::Uint8 *pixels;
    sf::Texture bufferTexture;
    sf::Sprite bufferSprite;
};

inline void MainWindow::drawPixel(
    const int x,
    const int y,
    const sf::Color *color,
    const int xSize)
{
    std::memcpy(pixels + (4 * (y * xSize + x)), color, 4);

    // const int point = 4 * (y * resolution.x + x);

    // pixels[point] = color.r;     // R
    // pixels[point + 1] = color.g; // G
    // pixels[point + 2] = color.b; // B
    // pixels[point + 3] = color.a; // A
}
