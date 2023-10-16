#pragma once

#include <Command.hpp>
#include <Scene.hpp>
#include <MainWindow.hpp>

class ResizeCommand : public Command
{
public:
    ResizeCommand(
        Scene &_scene,
        MainWindow &_mainWindow,
        const int _width,
        const int _height);

    void execute() override;

private:
    Scene &scene;
    MainWindow &mainWindow;

    const int width;
    const int height;
};
