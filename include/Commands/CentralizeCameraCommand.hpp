#pragma once

#include <Object.hpp>
#include <Camera.hpp>
#include <Command.hpp>

class CentralizeCameraCommand : public Command {
public:
    CentralizeCameraCommand(
            Camera &_camera,
            Object &_object);

    void execute() override;

private:
    Camera &camera;
    Object &object;
};
