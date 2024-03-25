#pragma once

#include <Sculptor.hpp>
#include <Scene.hpp>
#include <MainWindow.hpp>
#include <Command.hpp>
#include <memory>
#include <queue>

class Engine
{
public:
    Engine(Scene &_scene, MainWindow &_mainWindow, Sculptor &_sculptor);

    void start();

private:
    const int defaultFps = 165;

    Scene &scene;
    MainWindow &mainWindow;
    Sculptor &sculptor;

    AxisName moveAxis;
    Direction moveDirection;

    sf::Clock clock;
    int dt;

    std::queue<std::unique_ptr<Command>> commandsQueue;

    void handleEvents();

    void updateInput(const sf::Event &event);

    void sendInputCommand(const sf::Event &event);

    void update();

    void draw();
};
