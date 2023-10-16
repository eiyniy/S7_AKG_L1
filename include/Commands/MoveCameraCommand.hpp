#pragma once

#include <MoveCommand.hpp>

class MoveCameraCommand : public MoveCommand
{
public:
    MoveCameraCommand(
        Scene &_scene,
        const AxisName _axisName,
        const Direction _direction,
        const int _dt);

    void execute() override;
};
