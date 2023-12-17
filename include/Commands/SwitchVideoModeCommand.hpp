#pragma once

#include <Command.hpp>
#include <Point.hpp>
#include <Scene.hpp>
#include <MainWindow.hpp>

class SwitchVideoModeCommand : public Command {
public:
    SwitchVideoModeCommand(
            Camera &_camera,
            MainWindow &_mainWindow,
            bool _isEscape);

    ~SwitchVideoModeCommand() override = default;

    void execute() override;

private:
    Camera &camera;
    MainWindow &mainWindow;

    const bool isEscape;
};
