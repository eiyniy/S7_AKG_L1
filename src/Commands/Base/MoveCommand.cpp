#include <MoveCommand.hpp>

MoveCommand::MoveCommand(
    Scene &_scene,
    const AxisName _axisName,
    const Direction _direction,
    const int _dt)
    : TimedCommand(_dt),
      scene(_scene)
{
    transition = scene.cGetTransition(_axisName, _direction, dt);
}