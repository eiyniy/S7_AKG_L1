#include <iostream>
#include <Scene.hpp>

Scene::Scene(ObjInfo &p_objInfo, Camera &p_camera, const CoordinateVector &p_up)
    : objInfo(p_objInfo), camera(p_camera), up(p_up) {}

void Scene::modelConvert()
{
    std::vector<Vertex> &vertices = objInfo.getVertices();

    // CoordinateVector::getObserverConvert(camera.getPosition(), camera.getTarget(), up).log();
    // CoordinateVector::getProjectionConvert(camera.getFOV(), camera.getAspect(), 400, 1).log();
    // CoordinateVector::getWindowConvert(camera.cGetResolution().x, camera.cGetResolution().y, 0, 0).log();

    auto convertMatrix =
        CoordinateVector::getWindowConvert(camera.cGetResolution().x, camera.cGetResolution().y, 0, 0) *
        CoordinateVector::getProjectionConvert(camera.getFOV(), camera.getAspect(), 400, 1) *
        CoordinateVector::getObserverConvert(camera.getPosition(), camera.getTarget(), up);

    // convertMatrix.log();

    for (auto it = vertices.begin(); it < vertices.end(); ++it)
    {
        auto cv = CoordinateVector(it->getX(), it->getY(), it->getZ(), it->getW().value_or(1));

        /*
        // cv.log();
        cv = CoordinateVector::getObserverConvert(camera.getPosition(), camera.getTarget(), up) * (Matrix)cv;
        // cv.log();
        cv = CoordinateVector::getProjectionConvert(camera.getFOV(), camera.getAspect(), 400, 1) * (Matrix)cv;
        // cv.log();
        cv.getValue(0, 0) /= cv.getValue(3, 0);
        cv.getValue(1, 0) /= cv.getValue(3, 0);
        cv.getValue(2, 0) /= cv.getValue(3, 0);
        cv.getValue(3, 0) /= cv.getValue(3, 0);
        // cv.log();
        cv = CoordinateVector::getWindowConvert(camera.cGetResolution().x, camera.cGetResolution().y, 0, 0) * (Matrix)cv;
        // cv.log();
        */

        cv = convertMatrix * (Matrix)cv;
        // cv.log();
        cv.getValue(0, 0) /= cv.getValue(3, 0);
        cv.getValue(1, 0) /= cv.getValue(3, 0);
        cv.getValue(2, 0) /= cv.getValue(3, 0);
        cv.getValue(3, 0) /= cv.getValue(3, 0);
        // cv.log();

        *it = Vertex(BaseVertex::fromMatrix(cv));

        // std::cout << "it: " << it->getX() << " " << it->getY() << " " << it->getZ() << " " << it->getW().value_or(1) << std::endl;
    }

    std::cout << "after it: "
              << objInfo.cGetVertices().begin()->getX() << " "
              << objInfo.cGetVertices().begin()->getY() << " "
              << objInfo.cGetVertices().begin()->getZ() << " "
              << objInfo.cGetVertices().begin()->getW().value_or(1)
              << std::endl;
}

void Scene::moveConvert(AxisName axis, Direction direction)
{
    std::vector<Vertex> &vertices = objInfo.getVertices();

    std::cout << "camera: " << camera.cGetPosition().getX() << " "
              << camera.cGetPosition().getY() << " "
              << camera.cGetPosition().getZ() << " "
              << camera.cGetPosition().getW() << std::endl;

    CoordinateVector transition;

    switch (axis)
    {
    case AxisName::X:
        if (direction == Direction::Forward)
            transition = CoordinateVector(camera.getPosition().getX() + 10, 0, 0);
        else
            transition = CoordinateVector(camera.getPosition().getX() - 10, 0, 0);

        break;
    case AxisName::Y:
        if (direction == Direction::Forward)
            transition = CoordinateVector(0, camera.getPosition().getY() + 10, 0);
        else
            transition = CoordinateVector(0, camera.getPosition().getY() - 10, 0);

        break;
    case AxisName::Z:
        if (direction == Direction::Forward)
            transition = CoordinateVector(0, 0, camera.getPosition().getZ() + 10);
        else
            transition = CoordinateVector(0, 0, camera.getPosition().getZ() - 10);

        break;
    }

    camera.getPosition() = CoordinateVector::getMoveConvert(transition) * (Matrix)camera.getPosition();

    std::cout << "scene moved" << std::endl;
    std::cout << "camera: " << camera.cGetPosition().getX() << " "
              << camera.cGetPosition().getY() << " "
              << camera.cGetPosition().getZ() << " "
              << camera.cGetPosition().getW() << std::endl;
}

void Scene::scaleConvert()
{
}

void Scene::rotateConvert()
{
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
