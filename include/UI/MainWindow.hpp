#pragma once

#include <SFML/Graphics.hpp>
#include <Scene.hpp>

class MainWindow
{
public:
    MainWindow(Scene &p_scene);

    void startLoop();

private:
    void drawAllModels();
    void drawModel(const ObjInfo &objInfo, const std::vector<Vertex> &viewportVertices);

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
