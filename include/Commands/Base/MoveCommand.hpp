#pragma once

#include <Command.hpp>
#include <Enums.hpp>
#include <Scene.hpp>

class MoveCommand : public Command
{
protected:
    MoveCommand(
        const AxisName _axisName,
        const Direction _direction,
        const double _step);

protected:
    const AxisName axisName;
    const Direction direction;
    const double step;
};
