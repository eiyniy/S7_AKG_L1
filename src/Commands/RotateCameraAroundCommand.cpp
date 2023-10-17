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
    const double ratio = dt != 0 ? (dt / scene.defaultFrameTime) : 1.f;
    const double step = scene.rotationSpeed * ratio;

    bool isReversed = false;
    scene.getCamera().rotateAround(axisName, direction, step, isReversed);

    if (isReversed)
        scene.flip();
}
