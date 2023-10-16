#pragma once

#include <Command.hpp>

class TimedCommand : public Command
{
protected:
    TimedCommand(const int _dt);

protected:
    const int dt;
};
