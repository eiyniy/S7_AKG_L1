#pragma once

#include <MainWindow.hpp>
#include <Command.hpp>

class CloseCommand : public Command
{
public:
    CloseCommand(MainWindow &_mainWindow);

    void execute() override;

private:
    MainWindow &mainWindow;
};
