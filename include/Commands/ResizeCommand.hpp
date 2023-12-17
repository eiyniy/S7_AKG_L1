#pragma once

#include <Command.hpp>
#include <Scene.hpp>
#include <MainWindow.hpp>

class ResizeCommand : public Command {
public:
    ResizeCommand(
            Camera &_camera,
            MainWindow &_mainWindow,
            int _width,
            int _height);

    ~ResizeCommand() override = default;

    void execute() override;

private:
    Camera &camera;
    MainWindow &mainWindow;

    const int width;
    const int height;
};
