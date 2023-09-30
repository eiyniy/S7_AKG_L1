#pragma once

#include <SFML/Graphics.hpp>
#include <Scene.hpp>

class MainWindow
{
public:
    MainWindow(const Scene &p_scene);

    void startLoop();

private:
    const Scene &scene;

    sf::RenderWindow window;
    sf::Image buffer;
    sf::Texture bufferTexture;
    sf::Sprite bufferSprite;
};
