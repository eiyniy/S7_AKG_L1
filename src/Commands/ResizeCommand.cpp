#include <ResizeCommand.hpp>

ResizeCommand::ResizeCommand(
    Scene &_scene,
    MainWindow &_mainWindow,
    const int _width,
    const int _height)
    : scene(_scene),
      mainWindow(_mainWindow),
      width(_width),
      height(_height) {}

void ResizeCommand::execute()
{
    mainWindow.resize(width, height);
    scene.resize(width, height);
}
