#include <MoveCommand.hpp>

MoveCommand::MoveCommand(
    const AxisName _axisName,
    const Direction _direction,
    const double _step)
    : axisName(_axisName),
      direction(_direction),
      step(_step) {}