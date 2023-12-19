#include <MoveCommand.hpp>

MoveCommand::MoveCommand(
    const AxisName _axisName,
    const Direction _direction,
    const double _step)
    : axisName(_axisName),
      direction(_direction),
      step(_step) {}

Matrix<4, 1> MoveCommand::getTransition()
{
    Matrix<4, 1> transition;

    switch (axisName)
    {
    case AxisName::X:
        if (direction == Direction::Forward)
            transition = Matrix<4, 1>(step, 0, 0, 0);
        else
            transition = Matrix<4, 1>(-step, 0, 0, 0);

        break;
    case AxisName::Y:
        if (direction == Direction::Forward)
            transition = Matrix<4, 1>(0, step, 0, 0);
        else
            transition = Matrix<4, 1>(0, -step, 0, 0);

        break;
    case AxisName::Z:
        if (direction == Direction::Forward)
            transition = Matrix<4, 1>(0, 0, step, 0);
        else
            transition = Matrix<4, 1>(0, 0, -step, 0);

        break;
    }

    return transition;
}
