#pragma once

#include <MoveCommand.hpp>
#include <Object.hpp>

class MoveObjectCommand : public MoveCommand {
public:
    MoveObjectCommand(
            Object &_object,
            const AxisName _axisName,
            const Direction _direction,
            const double _step);

    void execute() override;

private:
    Object &object;
};
