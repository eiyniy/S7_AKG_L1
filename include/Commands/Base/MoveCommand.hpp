#pragma once

#include <Command.hpp>
#include <Enums.hpp>
#include <Scene.hpp>

class MoveCommand : public Command {
protected:
    MoveCommand(
            AxisName _axisName,
            Direction _direction,
            double _step);

    Matrix<4, 1> getTransition();

    const AxisName axisName;
    const Direction direction;
    const double step;
};
