#include <SwitchVideoModeCommand.hpp>

SwitchVideoModeCommand::SwitchVideoModeCommand(
        Camera &_camera,
        MainWindow &_mainWindow,
        const bool _isEscape)
        : camera(_camera),
          mainWindow(_mainWindow),
          isEscape(_isEscape) {}

void SwitchVideoModeCommand::execute() {
    mainWindow.switchVideoMode(isEscape);
    camera.setResolution(mainWindow.cGetResolution());
}
