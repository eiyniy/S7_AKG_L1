#pragma once

#include <Command.hpp>
#include <Types.hpp>
#include <Scene.hpp>

class MoveCommand : public Command {
protected:
    MoveCommand(
            AxisName _axisName,
            Direction _direction,
            double _step);

    Vector<4> getTransition();

    const AxisName axisName;
    const Direction direction;
    const double step;
};
