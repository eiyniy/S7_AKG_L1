#include <RotateCameraAroundCommand.hpp>

RotateCameraAroundCommand::RotateCameraAroundCommand(
    Scene &_scene,
    const AxisName _axisName,
    const Direction _direction,
    const int _dt)
    : TimedCommand(_dt),
      scene(_scene),
      axisName(_axisName),
      direction(_direction) {}

void RotateCameraAroundCommand::execute()
{
    scene.rotateCameraAround(axisName, direction, dt);
}
