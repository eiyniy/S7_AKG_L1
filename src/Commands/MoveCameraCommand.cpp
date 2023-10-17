#include <MoveCameraCommand.hpp>

MoveCameraCommand::MoveCameraCommand(
    Scene &_scene,
    const AxisName _axisName,
    const Direction _direction,
    const int _dt)
    : MoveCommand(_scene, _axisName, _direction, _dt) {}

void MoveCameraCommand::execute()
{
    scene.getCamera().move(transition);
}
