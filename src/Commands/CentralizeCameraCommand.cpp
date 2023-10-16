#include <CentralizeCameraCommand.hpp>

CentralizeCameraCommand::CentralizeCameraCommand(Scene &_scene)
    : scene(_scene) {}

void CentralizeCameraCommand::execute()
{
    scene.centralizeCamera();
}
