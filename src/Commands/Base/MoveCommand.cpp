#include <MoveCommand.hpp>

MoveCommand::MoveCommand(
    const AxisName _axisName,
    const Direction _direction,
    const double _step)
    : axisName(_axisName),
      direction(_direction),
      step(_step) {}

Vector<4> MoveCommand::getTransition()
{
    Vector<4> transition;

    switch (axisName)
    {
    case AxisName::X:
        if (direction == Direction::Forward)
            transition = Vector<4>(step, 0, 0, 0);
        else
            transition = Vector<4>(-step, 0, 0, 0);

        break;
    case AxisName::Y:
        if (direction == Direction::Forward)
            transition = Vector<4>(0, step, 0, 0);
        else
            transition = Vector<4>(0, -step, 0, 0);

        break;
    case AxisName::Z:
        if (direction == Direction::Forward)
            transition = Vector<4>(0, 0, step, 0);
        else
            transition = Vector<4>(0, 0, -step, 0);

        break;
    }

    return transition;
}
