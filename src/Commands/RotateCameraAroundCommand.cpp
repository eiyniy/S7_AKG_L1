#include <RotateCameraAroundCommand.hpp>

RotateCameraAroundCommand::RotateCameraAroundCommand(
    Camera &_camera,
    const AxisName _axisName,
    const Direction _direction,
    const double _step)
    : camera(_camera),
      axisName(_axisName),
      direction(_direction),
      step(_step) {}

void RotateCameraAroundCommand::execute()
{
    camera.rotateAround(axisName, direction, step);
}
