#pragma once

#include <MoveCommand.hpp>
#include <Camera.hpp>

class MoveCameraCommand : public MoveCommand {
public:
    MoveCameraCommand(
            Camera &_camera,
            AxisName _axisName,
            Direction _direction,
            double _step);

    ~MoveCameraCommand() override = default;

    void execute() override;

private:
    Camera &camera;
};
