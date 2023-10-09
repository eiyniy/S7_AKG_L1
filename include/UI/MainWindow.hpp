#pragma once

#include <SFML/Graphics.hpp>
#include <Scene.hpp>

class MainWindow
{
public:
    MainWindow(Scene &p_scene);

    void startLoop();

private:
    void draw();

    bool isCameraMoving;
    bool isCameraRotating;
    bool isCameraRotatingAround;
    bool isObjectMoving;
    bool isCentering;

    Scene &scene;

    sf::RenderWindow window;
    sf::Uint8 *pixels;
    sf::Texture bufferTexture;
    sf::Sprite bufferSprite;
};
