#include <iostream>
#include <Scene.hpp>

Scene::Scene(
    ObjInfo &p_objInfo,
    Camera &p_camera,
    const CoordinateVector &p_up,
    const int p_moveSpeed)
    : cObjInfo(p_objInfo),
      camera(p_camera),
      up(p_up),
      moveSpeed(p_moveSpeed)
{
    objInfo = ObjInfo(cObjInfo);
}

void Scene::modelConvert()
{
    std::vector<Vertex> &vertices = getObjInfoCopy().getVertices();

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

        *it = Vertex(BaseVertex::fromMatrix(cv));
    }
}

void Scene::moveConvert(AxisName axis, Direction direction, int dt)
{
    std::vector<Vertex> &vertices = getObjInfoCopy().getVertices();

    std::cout << "first: " << objInfo.cGetVertices().begin()->cGetX() << " "
              << objInfo.cGetVertices().begin()->cGetY() << " "
              << objInfo.cGetVertices().begin()->cGetZ() << " "
              << objInfo.cGetVertices().begin()->cGetW() << std::endl;

    CoordinateVector transition;

    double moveSpeedTimed = moveSpeed * dt != 0 ? (dt / defaultFrameTime) : 1;
    std::cout << "speed " << moveSpeedTimed << std::endl;

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

    auto mConvert = CoordinateVector::getMoveConvert(transition);

    camera.getTarget() = mConvert * (Matrix)camera.getTarget();
    camera.getPosition() = mConvert * (Matrix)camera.getPosition();
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
