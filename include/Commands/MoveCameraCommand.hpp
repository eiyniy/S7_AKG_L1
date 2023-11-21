#pragma once

#include <MoveCommand.hpp>
#include <Camera.hpp>

class MoveCameraCommand : public MoveCommand {
public:
    MoveCameraCommand(
            Camera &_camera,
            const AxisName _axisName,
            const Direction _direction,
            const double _step);

    void execute() override;

private:
    Camera &camera;
};
