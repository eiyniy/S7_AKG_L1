#include <iostream>
#include <Scene.hpp>
#include <Converter.hpp>

Scene::Scene(
    ObjInfo &p_objInfo,
    Camera &p_camera,
    const CoordinateVector &p_up,
    const double p_moveSpeed)
    : cObjInfo(p_objInfo),
      camera(p_camera),
      up(p_up),
      moveSpeed(p_moveSpeed),
      worldShift(0, 0, 0, 1)
{
    cFloor.emplace_back(Vertex(-100, 0, 0));
    cFloor.emplace_back(Vertex(0, 0, 0));
    cFloor.emplace_back(Vertex(100, 0, 0));
    cFloor.emplace_back(Vertex(0, 0, 0));
    cFloor.emplace_back(Vertex(0, -100, 0));
    cFloor.emplace_back(Vertex(0, 0, 0));
    cFloor.emplace_back(Vertex(0, 100, 0));
    cFloor.emplace_back(Vertex(0, 0, 0));
    cFloor.emplace_back(Vertex(0, 0, -100));
    cFloor.emplace_back(Vertex(0, 0, 0));
    cFloor.emplace_back(Vertex(0, 0, 100));
    cFloor.emplace_back(Vertex(0, 0, 0));
}

void Scene::modelConvert(const std::vector<Vertex> &vertices, const std::optional<CoordinateVector> &moveConvert)
{
    objInfoVertices = std::vector<Vertex>();
    objInfoVertices.reserve(vertices.size());

    auto convertMatrix =
        Matrix::getProjectionConvert(camera.getFOV(), camera.getAspect(), 2, 1) *
        Matrix::getObserverConvert(camera.getPosition(), camera.cGetTarget(), up);

    if (moveConvert.has_value())
        convertMatrix = convertMatrix * Matrix::getMoveConvert(*moveConvert);

    static CoordinateVector cv;

    for (auto it = vertices.begin(); it < vertices.end(); ++it)
    {
        cv = Converter::vertexToCVector(*it);

        cv = convertMatrix * (Matrix)cv;

        if (cv.cGetW() < 0)
            continue;

        cv = cv / cv.cGetW();

        if (cv.cGetX() < -1 || cv.cGetX() > 1 ||
            cv.cGetY() < -1 || cv.cGetY() > 1) // ||
            // cv.cGetZ() < 0 || cv.cGetZ() > 1)
            continue;

        cv = Matrix::getWindowConvert(camera.cGetResolution().x, camera.cGetResolution().y, 0, 0) * (Matrix)cv;

        objInfoVertices.emplace_back(Converter::cVectorToVertex(cv));
    }
}

void Scene::moveCamera(CoordinateVector &transition)
{
    auto mConvert = CoordinateVector::getMoveConvert(transition);
    mConvert.getValue(3, 3) = 1;

    camera.getTarget() = mConvert * (Matrix)camera.getTarget();
    camera.getPosition() = mConvert * (Matrix)camera.getPosition();
}

CoordinateVector Scene::getMoveConvert(AxisName axis, Direction direction, int dt)
{
    CoordinateVector transition;

    double ratio = dt != 0 ? ((double)dt / defaultFrameTime) : 1.f;
    double moveSpeedTimed = moveSpeed * ratio;

    switch (axis)
    {
    case AxisName::X:
        if (direction == Direction::Forward)
            transition = CoordinateVector(moveSpeedTimed, 0, 0, 0);
        else
            transition = CoordinateVector(-moveSpeedTimed, 0, 0, 0);

        break;
    case AxisName::Y:
        if (direction == Direction::Forward)
            transition = CoordinateVector(0, moveSpeedTimed, 0, 0);
        else
            transition = CoordinateVector(0, -moveSpeedTimed, 0, 0);

        break;
    case AxisName::Z:
        if (direction == Direction::Forward)
            transition = CoordinateVector(0, 0, moveSpeedTimed, 0);
        else
            transition = CoordinateVector(0, 0, -moveSpeedTimed, 0);

        break;
    }

    return transition;
}

const std::vector<Vertex> &Scene::cGetObjInfoVerticesCopy() const
{
    return cObjInfo.cGetVertices();
}

std::vector<Vertex> &Scene::getFloorCopy()
{
    floor = std::vector<Vertex>(cFloor);
    return floor;
}

const std::vector<Vertex> &Scene::cGetObjInfoVertices() const
{
    return objInfoVertices;
}

const Camera &Scene::cGetCamera() const
{
    return camera;
}

const CoordinateVector &Scene::cGetWorldShift() const
{
    return worldShift;
}

Camera &Scene::getCamera()
{
    return camera;
}

CoordinateVector &Scene::getWorldShift()
{
    return worldShift;
}

std::vector<std::array<sf::Vertex, 2>> Scene::getDrawableFloor()
{
    auto res = std::vector<std::array<sf::Vertex, 2>>(floor.size() / 2);

    for (int i = 0, j = 0; i < floor.size() && j < res.size(); i += 2, ++j)
    {
        res[j][0] = sf::Vertex(sf::Vector2f(floor[i].cGetX(), floor[i].cGetY()));
        res[j][1] = sf::Vertex(sf::Vector2f(floor[i + 1].cGetX(), floor[i + 1].cGetY()));
    }

    return res;
}
