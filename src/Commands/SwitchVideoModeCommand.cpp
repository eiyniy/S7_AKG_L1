#include <SwitchVideoModeCommand.hpp>

SwitchVideoModeCommand::SwitchVideoModeCommand(
    Scene &_scene,
    MainWindow &_mainWindow,
    const bool _isEscape)
    : scene(_scene),
      mainWindow(_mainWindow),
      isEscape(_isEscape) {}

void SwitchVideoModeCommand::execute()
{
    mainWindow.switchVideoMode(isEscape);
    scene.getCamera().setResolution(mainWindow.cGetResolution());
}
