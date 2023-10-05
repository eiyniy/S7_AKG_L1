#include <iostream>
#include <Scene.hpp>
#include <Converter.hpp>

Scene::Scene(
    ObjInfo &p_objInfo,
    Camera &p_camera,
    const CoordinateVector &p_up,
    const int p_moveSpeed)
    : cObjInfo(p_objInfo),
      camera(p_camera),
      up(p_up),
      moveSpeed(p_moveSpeed),
      worldShift(0, 0, 0, 1)
{
    objInfo = ObjInfo(cObjInfo);
}

void Scene::modelConvert(std::vector<Vertex> &vertices)
{
    auto convertMatrix =
        CoordinateVector::getWindowConvert(camera.cGetResolution().x, camera.cGetResolution().y, 0, 0) *
        CoordinateVector::getProjectionConvert(camera.getFOV(), camera.getAspect(), 2, 1) *
        CoordinateVector::getObserverConvert(camera.getPosition(), camera.cGetTarget(), up);

    for (auto it = vertices.begin(); it < vertices.end(); ++it)
    {
        auto cv = CoordinateVector(it->getX(), it->getY(), it->getZ(), it->getW());

        cv = convertMatrix * (Matrix)cv;

        cv.getValue(0, 0) /= cv.getValue(3, 0);
        cv.getValue(1, 0) /= cv.getValue(3, 0);
        cv.getValue(2, 0) /= cv.getValue(3, 0);
        cv.getValue(3, 0) /= cv.getValue(3, 0);

        *it = Converter::coordinateVectorToVertex(cv);
    }
}

void Scene::moveConvert(AxisName axis, Direction direction, int dt)
{
    std::vector<Vertex> &vertices = getObjInfoCopy().getVertices();

    CoordinateVector transition;

    double moveSpeedTimed = moveSpeed; // * dt != 0 ? (dt / defaultFrameTime) : 1;

    switch (axis)
    {
    case AxisName::X:
        if (direction == Direction::Forward)
            transition = CoordinateVector(moveSpeedTimed, 0, 0, 1);
        else
            transition = CoordinateVector(-moveSpeedTimed, 0, 0, 1);

        break;
    case AxisName::Y:
        if (direction == Direction::Forward)
            transition = CoordinateVector(0, moveSpeedTimed, 0, 1);
        else
            transition = CoordinateVector(0, -moveSpeedTimed, 0, 1);

        break;
    case AxisName::Z:
        if (direction == Direction::Forward)
            transition = CoordinateVector(0, 0, moveSpeedTimed, 1);
        else
            transition = CoordinateVector(0, 0, -moveSpeedTimed, 1);

        break;
    }

    worldShift.getX() += transition.cGetX();
    worldShift.getY() += transition.cGetY();
    worldShift.getZ() += transition.cGetZ();
    // worldShift.getW() += transition.cGetW();

    // auto mConvert = CoordinateVector::getMoveConvert(transition);

    // if (axis == AxisName::X || axis == AxisName::Y)
    // camera.getTarget() = mConvert * (Matrix)camera.getTarget();
    // else
    // camera.getPosition() = mConvert * (Matrix)camera.getPosition();

    auto convertMatrix =
        CoordinateVector::getWindowConvert(camera.cGetResolution().x, camera.cGetResolution().y, 0, 0) *
        CoordinateVector::getProjectionConvert(camera.getFOV(), camera.getAspect(), 2, 1) *
        CoordinateVector::getObserverConvert(camera.getPosition(), camera.cGetTarget(), up) *
        CoordinateVector::getMoveConvert(worldShift);

    for (auto it = vertices.begin(); it < vertices.end(); ++it)
    {
        auto cv = CoordinateVector(it->getX(), it->getY(), it->getZ(), it->getW());

        cv = convertMatrix * (Matrix)cv;

        cv.getValue(0, 0) /= cv.getValue(3, 0);
        cv.getValue(1, 0) /= cv.getValue(3, 0);
        cv.getValue(2, 0) /= cv.getValue(3, 0);
        cv.getValue(3, 0) /= cv.getValue(3, 0);

        *it = Converter::coordinateVectorToVertex(cv);
    }
}

void Scene::scaleConvert()
{
}

void Scene::rotateConvert()
{
}

ObjInfo &Scene::getObjInfoCopy()
{
    objInfo = ObjInfo(cObjInfo);
    return objInfo;
}

const ObjInfo &Scene::cGetObjInfo() const
{
    return objInfo;
}

const Camera &Scene::cGetCamera() const
{
    return camera;
}

Camera &Scene::getCamera()
{
    return camera;
}

std::vector<std::array<sf::Vertex, 2>> Scene::getFloor()
{
    auto res = std::vector<std::array<sf::Vertex, 2>>(floor.size());

    for (int i = 0; i < floor.size(); ++i)
    {
        // floor[i] = Vertex(floor[i].cGetX(), floor[i].cGetX(), floor[i].cGetX(), floor[i].cGetX());
    }

    // modelConvert(floor.)
}
