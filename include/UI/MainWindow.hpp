#pragma once

#include <SFML/Graphics.hpp>
#include <Scene.hpp>

class MainWindow
{
public:
    MainWindow(Scene &p_scene);

    void startLoop();

private:
    void drawLineBR_1(
        sf::Uint8 *pixels,
        const Dot p1,
        const Dot p2,
        const sf::Color color);
    void drawLineDDA(
        sf::Uint8 *pixels,
        const Dot p1,
        const Dot p2,
        const sf::Color color);

    void drawAllModels();
    void drawModel(const ObjInfo &objInfo, const std::vector<Vertex> &viewportVertices);
    void drawPixel(
        const int x,
        const int y,
        sf::Color color,
        const Dot resolution);

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
