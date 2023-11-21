#pragma once

#include <Command.hpp>
#include <Enums.hpp>
#include <Camera.hpp>

class RotateCameraAroundCommand : public Command {
public:
    RotateCameraAroundCommand(
            Camera &_camera,
            const AxisName _axisName,
            const Direction _direction,
            const double _step);

    void execute() override;

protected:
    Camera &camera;

    const AxisName axisName;
    const Direction direction;
    const double step;
};
