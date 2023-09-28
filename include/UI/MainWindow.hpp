#pragma once

#include <SFML/Graphics.hpp>
#include <Scene.hpp>

class MainWindow
{
public:
    MainWindow(Scene &p_scene);

    void startLoop();

private:
    Scene &scene;

    sf::RenderWindow window;
    sf::Image buffer;
    sf::Texture bufferTexture;
    sf::Sprite bufferSprite;
};
