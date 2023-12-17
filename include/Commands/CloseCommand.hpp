#pragma once

#include <MainWindow.hpp>
#include <Command.hpp>

class CloseCommand : public Command {
public:
    explicit CloseCommand(MainWindow &_mainWindow);

    ~CloseCommand() override = default;
 
    void execute() override;

private:
    MainWindow &mainWindow;
};
