#include <iostream>
#include <Scene.hpp>
#include <Converter.hpp>
#include <Timer.hpp>
#include <Math.hpp>

Scene::Scene(
    ObjInfo &p_objInfo,
    Camera &p_camera,
    const CoordinateVector &p_up,
    const double p_moveSpeed)
    : cObjInfo(p_objInfo),
      camera(p_camera),
      up(p_up),
      moveSpeed(p_moveSpeed),
      worldShift(0, 0, 0, 1),
      objInfoVertices(),
      floorVertices()
{
    generateFloor(30, 10);
}

void Scene::generateFloor(const int size, const int step)
{
    const auto evenSize = size % 2 == 0 ? size + 1 : size;
    const auto halfSize = evenSize / 2;

    for (int i = -halfSize; i <= halfSize; ++i)
    {
        for (int j = -halfSize; j <= halfSize; ++j)
            cFloor.addVertex(Vertex(j * step, 0, i * step));
    }

    for (int i = 0; i < evenSize - 1; ++i)
    {
        for (int j = 0; j < evenSize - 1; ++j)
        {
            auto vertexesIndexes = std::vector<VertexIndexes>();
            vertexesIndexes.reserve(4);

            vertexesIndexes.emplace_back(VertexIndexes(j * evenSize + i + 1));
            vertexesIndexes.emplace_back(VertexIndexes(j * evenSize + i + 2));
            vertexesIndexes.emplace_back(VertexIndexes((j + 1) * evenSize + i + 2));
            vertexesIndexes.emplace_back(VertexIndexes((j + 1) * evenSize + i + 1));

            cFloor.addPolygon(Polygon(vertexesIndexes));
        }
    }
}

// TODO: Replace rotateConvert with RotateInfo type {AxisName & angle}
void Scene::modelConvert(
    const std::vector<Vertex> &vertices,
    std::vector<Vertex> &drawVertices,
    const std::optional<CoordinateVector> &moveConvert)
{
    drawVertices.clear();
    drawVertices.reserve(vertices.size());

    auto convertMatrix =
        Matrix<4, 4>::getProjectionConvert(camera.getFOV(), camera.getAspect(), 1500, 0.1) *
        Matrix<4, 4>::getObserverConvert(camera.getPosition(), camera.cGetTarget(), up);

    if (moveConvert.has_value())
        convertMatrix = convertMatrix * Matrix<4, 4>::getMoveConvert(*moveConvert);

    static CoordinateVector cv;

    for (auto it = vertices.begin(); it < vertices.end(); ++it)
    {
        bool isOutOfScreen = false;
        bool isWNegative = false;

        cv = Converter::vertexToCVector(*it);

        cv = Converter::matrixToCVector(convertMatrix * Converter::cVectorToMatrix(cv));

        if (cv.cGetW() < 0)
            isWNegative = true;

        cv = cv / cv.cGetW();

        if (cv.cGetX() < -1 || cv.cGetX() > 1 ||
            cv.cGetY() < -1 || cv.cGetY() > 1 ||
            cv.cGetZ() < 0 || cv.cGetZ() > 1)
            isOutOfScreen = true;

        cv = Converter::matrixToCVector(Matrix<4, 4>::getWindowConvert(camera.cGetResolution().x, camera.cGetResolution().y, 0, 0) * cv);

        drawVertices.emplace_back(Converter::cVectorToVertex(cv, isOutOfScreen, isWNegative));
    }
}

void Scene::centralizeCamera()
{
    camera.getTarget() = Converter::vertexToCVector(cObjInfo.cGetCenter()) + cGetWorldShift();
}

void Scene::rotateCamera(const AxisName axisName, const double angle)
{
    auto rConvert = CoordinateVector::getRotateConvert(axisName, angle);

    camera.getTarget() = Converter::matrixToCVector(rConvert * camera.cGetTarget());
}

void Scene::rotateCameraAround(const AxisName axisName, const double angle)
{
    auto cameraRelative = Converter::matrixToCVector(camera.cGetPosition() - camera.cGetTarget());
    auto spherical = Math::decartToSpherical(cameraRelative);

    switch (axisName)
    {
    case AxisName::X:
        spherical.a = spherical.a + angle;
        break;
    case AxisName::Y:
        spherical.b = spherical.b + angle;
        break;
    }

    if (spherical.a < 0)
        spherical.a = 180;
    if (spherical.a > 180)
        spherical.a = 0;

    if (spherical.b < 0)
        spherical.b = 360;
    if (spherical.b > 360)
        spherical.a = 0;

    cameraRelative = Math::sphericalToDecart(spherical);
    camera.getPosition() = cameraRelative + camera.cGetTarget();
}

void Scene::moveCamera(const CoordinateVector &transition)
{
    camera.getTarget() = camera.cGetTarget() + transition;
    camera.getPosition() = camera.cGetPosition() + transition;
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

const ObjInfo &Scene::cGetFloor() const
{
    return cFloor;
}

std::vector<Vertex> &Scene::getFloorVertices()
{
    return floorVertices;
}

const std::vector<Vertex> &Scene::cGetFloorVertices() const
{
    return floorVertices;
}

const std::vector<Vertex> &Scene::cGetFloorVerticesCopy() const
{
    return cFloor.cGetVertices();
}

const std::vector<Vertex> &Scene::cGetObjInfoVertices() const
{
    return objInfoVertices;
}

const ObjInfo &Scene::cGetObjInfo() const
{
    return cObjInfo;
}

std::vector<Vertex> &Scene::getObjInfoVertices()
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
