#pragma once

#include <Command.hpp>
#include <Point.hpp>
#include <Scene.hpp>
#include <MainWindow.hpp>

class SwitchVideoModeCommand : public Command {
public:
    SwitchVideoModeCommand(
            Scene &_scene,
            MainWindow &_mainWindow,
            const bool _isEscape);

    void execute() override;

private:
    Scene &scene;
    MainWindow &mainWindow;

    const bool isEscape;
};
