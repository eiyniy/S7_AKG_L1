#pragma once

#include <Command.hpp>
#include <Scene.hpp>
#include <MainWindow.hpp>

class ResizeCommand : public Command {
public:
    ResizeCommand(
            Camera &_camera,
            MainWindow &_mainWindow,
            const int _width,
            const int _height);

    void execute() override;

private:
    Camera &camera;
    MainWindow &mainWindow;

    const int width;
    const int height;
};
