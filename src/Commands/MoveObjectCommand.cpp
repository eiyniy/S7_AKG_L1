#include <MoveObjectCommand.hpp>

MoveObjectCommand::MoveObjectCommand(
    Scene &_scene,
    const std::string &_objectName,
    const AxisName _axisName,
    const Direction _direction,
    const int _dt)
    : MoveCommand(_scene, _axisName, _direction, _dt),
      objectName(_objectName) {}

void MoveObjectCommand::execute()
{
    scene.moveObject(objectName, transition);
}
