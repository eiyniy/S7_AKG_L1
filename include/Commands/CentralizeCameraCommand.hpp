#pragma once

#include <Scene.hpp>
#include <Command.hpp>

class CentralizeCameraCommand : public Command
{
public:
    CentralizeCameraCommand(Scene &_scene);

    void execute() override;

private:
    Scene &scene;
};
