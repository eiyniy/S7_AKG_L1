#include <MoveObjectCommand.hpp>
#include <Engine.hpp>

MoveObjectCommand::MoveObjectCommand(
        Object &_object,
        const AxisName _axisName,
        const Direction _direction,
        const double _step)
        : MoveCommand(_axisName, _direction, _step),
          object(_object) {}

void MoveObjectCommand::execute() {
    object.move(getTransition());
}
