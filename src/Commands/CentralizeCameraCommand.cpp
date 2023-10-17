#include <CentralizeCameraCommand.hpp>
#include <Converter.hpp>

CentralizeCameraCommand::CentralizeCameraCommand(Camera &_camera, Object &_object)
    : camera(_camera), object(_object) {}

void CentralizeCameraCommand::execute()
{
    camera.setTarget(
        Converter::vertexToMatrix(
            object.getCenter()) +
        object.cGetShift());
}
