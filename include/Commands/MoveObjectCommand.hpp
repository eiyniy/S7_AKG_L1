#pragma once

#include <MoveCommand.hpp>
#include <Object.hpp>

class MoveObjectCommand : public MoveCommand {
public:
    MoveObjectCommand(
            Object &_object,
            AxisName _axisName,
            Direction _direction,
            double _step);

    ~MoveObjectCommand() override = default;

    void execute() override;

private:
    Object &object;
};
