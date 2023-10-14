#pragma once

#include <Scene.hpp>
#include <MainWindow.hpp>

class Engine
{
public:
    Engine(Scene &p_scene, MainWindow &p_mainWindow);

    void start();

private:
    const int defaultFps = 165;

    Scene &scene;
    MainWindow &mainWindow;

    bool isCameraMoving;
    bool isCameraRotating;
    bool isCameraRotatingAround;
    bool isObjectMoving;
    bool isCentering;

    sf::Event event;

    AxisName moveAxis;
    Direction moveDirection;

    sf::Clock clock;
    double dt;

    void handleEvents();
    void update();
    void draw();
};
