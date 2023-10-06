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

    cFloor.push_back(Vertex(-100, 0, 0));
    cFloor.push_back(Vertex(0, 0, 0));
    cFloor.push_back(Vertex(100, 0, 0));
    cFloor.push_back(Vertex(0, 0, 0));
    cFloor.push_back(Vertex(0, -100, 0));
    cFloor.push_back(Vertex(0, 0, 0));
    cFloor.push_back(Vertex(0, 100, 0));
    cFloor.push_back(Vertex(0, 0, 0));
    cFloor.push_back(Vertex(0, 0, -100));
    cFloor.push_back(Vertex(0, 0, 0));
    cFloor.push_back(Vertex(0, 0, 100));
    cFloor.push_back(Vertex(0, 0, 0));
}

void Scene::modelConvert(std::vector<Vertex> &vertices, std::optional<CoordinateVector> moveConvert)
{
    auto convertMatrix =
        Matrix::getWindowConvert(camera.cGetResolution().x, camera.cGetResolution().y, 0, 0) *
        Matrix::getProjectionConvert(camera.getFOV(), camera.getAspect(), 2, 1) *
        Matrix::getObserverConvert(camera.getPosition(), camera.cGetTarget(), up);

    if (moveConvert.has_value())
        convertMatrix = convertMatrix * Matrix::getMoveConvert(moveConvert.value());

    for (auto it = vertices.begin(); it < vertices.end(); ++it)
    {
        auto cv = CoordinateVector(it->getX(), it->getY(), it->getZ(), it->getW());

        cv = convertMatrix * (Matrix)cv;

        cv.getValue(0, 0) /= cv.getValue(3, 0);
        cv.getValue(1, 0) /= cv.getValue(3, 0);
        cv.getValue(2, 0) /= cv.getValue(3, 0);
        cv.getValue(3, 0) /= cv.getValue(3, 0);

        *it = Converter::cVectorToVertex(cv);
    }
}

CoordinateVector Scene::getMoveConvert(AxisName axis, Direction direction, int dt)
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

    // worldShift.getX() += transition.cGetX();
    // worldShift.getY() += transition.cGetY();
    // worldShift.getZ() += transition.cGetZ();
    // worldShift.getW() += transition.cGetW();

    auto mConvert = CoordinateVector::getMoveConvert(transition);

    // if (axis == AxisName::X || axis == AxisName::Y)
    camera.getTarget() = mConvert * (Matrix)camera.getTarget();
    // else
    camera.getPosition() = mConvert * (Matrix)camera.getPosition();

    return worldShift;
}

ObjInfo &Scene::getObjInfoCopy()
{
    objInfo = ObjInfo(cObjInfo);
    return objInfo;
}

std::vector<Vertex> &Scene::getFloorCopy()
{
    floor = std::vector<Vertex>(cFloor);
    return floor;
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

std::vector<std::array<sf::Vertex, 2>> Scene::getDrawableFloor()
{
    auto res = std::vector<std::array<sf::Vertex, 2>>(floor.size() / 2);

    for (int i = 0, j = 0; i < floor.size(), j < res.size(); i += 2, ++j)
    {
        res[j][0] = sf::Vertex(sf::Vector2f(floor[i].cGetX(), floor[i].cGetY()));
        res[j][1] = sf::Vertex(sf::Vector2f(floor[i + 1].cGetX(), floor[i + 1].cGetY()));
    }

    return res;
}
