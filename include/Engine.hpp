#pragma once

#include <Scene.hpp>
#include <MainWindow.hpp>
#include <TSQueue.hpp>
#include <Command.hpp>
#include <memory>

class Engine
{
public:
    Engine(Scene &_scene, MainWindow &_mainWindow);

    void start();

    static Matrix<4, 1> getTransition(
        const AxisName axis,
        const Direction direction,
        const double step);

private:
    const int defaultFps = 165;

    Scene &scene;
    MainWindow &mainWindow;

    AxisName moveAxis;
    Direction moveDirection;

    sf::Clock clock;
    int dt;

    TSQueue<std::unique_ptr<Command>> commandsQueue;

    void handleEvents();
    void updateInput(const sf::Event &event);
    void sendInputCommand(const sf::Event &event);
    void update();
    void draw();
};
