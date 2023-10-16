#pragma once

#include <TimedCommand.hpp>
#include <Enums.hpp>
#include <Scene.hpp>

class RotateCameraAroundCommand : public TimedCommand
{
public:
    RotateCameraAroundCommand(
        Scene &_scene,
        const AxisName _axisName,
        const Direction _direction,
        const int _dt);

    void execute() override;

protected:
    Scene &scene;

    const AxisName axisName;
    const Direction direction;
};
