#include <MoveCameraCommand.hpp>
#include <Engine.hpp>

MoveCameraCommand::MoveCameraCommand(
    Camera &_camera,
    const AxisName _axisName,
    const Direction _direction,
    const double _step)
    : MoveCommand(_axisName, _direction, _step),
      camera(_camera) {}

void MoveCameraCommand::execute()
{
    auto transition = Engine::getTransition(axisName, direction, step);
    camera.move(transition);
}
