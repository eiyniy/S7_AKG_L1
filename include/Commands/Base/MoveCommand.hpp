#pragma once

#include <TimedCommand.hpp>
#include <Enums.hpp>
#include <Scene.hpp>

class MoveCommand : public TimedCommand
{
protected:
    MoveCommand(
        Scene &_scene,
        const AxisName _axisName,
        const Direction _direction,
        const int _dt);

protected:
    Scene &scene;

    Matrix<4, 1> transition;
};
