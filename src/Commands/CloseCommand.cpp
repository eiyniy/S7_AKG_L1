#include <CloseCommand.hpp>

CloseCommand::CloseCommand(MainWindow &_mainWindow)
    : mainWindow(_mainWindow) {}

void CloseCommand::execute()
{
    mainWindow.getWindow().close();
}
