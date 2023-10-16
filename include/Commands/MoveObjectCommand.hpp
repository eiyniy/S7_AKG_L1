#pragma once

#include <MoveCommand.hpp>

class MoveObjectCommand : public MoveCommand
{
public:
    MoveObjectCommand(
        Scene &_scene,
        const std::string &_objectName,
        const AxisName _axisName,
        const Direction _direction,
        const int _dt);

    void execute() override;

private:
    const std::string &objectName;
};
