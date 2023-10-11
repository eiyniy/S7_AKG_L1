#include <iostream>
#include <Scene.hpp>
#include <Converter.hpp>
#include <Timer.hpp>
#include <Math.hpp>

Scene::Scene(
    Camera &p_camera,
    CoordinateVector &p_up,
    const double p_moveSpeed,
    const double p_rotationSpeed)
    : camera(p_camera),
      up(p_up),
      moveSpeed(p_moveSpeed),
      rotationSpeed(p_rotationSpeed)
{
}

Scene::~Scene()
{
    for (auto pair : objects)
        delete pair.second;
}

void Scene::generateFloor()
{
    double maxDimensionSize = 0;

    for (auto pair : objects)
    {
        if (pair.first == floorObjectName)
            continue;

        auto maxXZ = Converter::vertexToCVector(pair.second->getMaxXZ());
        auto minXZ = Converter::vertexToCVector(pair.second->getMinXZ());

        auto floorSize = Converter::matrixToCVector(maxXZ - minXZ);

        auto biggerDimensionSize =
            floorSize.cGetX() > floorSize.cGetZ()
                ? floorSize.cGetX()
                : floorSize.cGetZ();

        if (biggerDimensionSize > maxDimensionSize)
            maxDimensionSize = biggerDimensionSize;
    }

    maxDimensionSize *= 2;

    auto step = maxDimensionSize / floorStepsCount;
    generateFloor(floorStepsCount, ceil(step));
}

void Scene::generateFloor(const int size, const int step)
{
    auto color = sf::Color(255U, 255U, 255U, 64U);
    auto floorPt = new ObjInfo(color);

    objects.insert_or_assign(floorObjectName, floorPt);

    objectsConvertedVertices.insert_or_assign(
        floorObjectName,
        std::vector<Vertex>(floorPt->cGetVertices().size()));

    objectsShift.insert_or_assign(
        floorObjectName,
        CoordinateVector(0, 0, 0, 1));

    const auto evenSize = size % 2 == 0 ? size + 1 : size;
    const auto halfSize = evenSize / 2;

    for (int i = -halfSize; i <= halfSize; ++i)
    {
        for (int j = -halfSize; j <= halfSize; ++j)
            floorPt->addVertex(Vertex(j * step, 0, i * step));
    }

    for (int i = 0; i < evenSize - 1; ++i)
    {
        for (int j = 0; j < evenSize - 1; ++j)
        {
            auto vertexesIndexes = std::vector<VertexIndexes>{
                VertexIndexes(j * evenSize + i + 1),
                VertexIndexes(j * evenSize + i + 2),
                VertexIndexes((j + 1) * evenSize + i + 2),
                VertexIndexes((j + 1) * evenSize + i + 1)};

            floorPt->addPolygon(Polygon(vertexesIndexes));
        }
    }
}

void Scene::convertAllModels()
{
    for (auto pair : objects)
    {
        if (pair.first == floorObjectName)
        {
            convertModel(
                objectsConvertedVertices.at(floorObjectName),
                objects.at(floorObjectName)->cGetVertices());
        }
        else
        {
            convertModel(
                objectsConvertedVertices.at(selectedObjectName),
                objects.at(selectedObjectName)->cGetVertices(),
                objectsShift.at(selectedObjectName));
        }
    }
}

// TODO: Replace rotateConvert with RotateInfo type {AxisName & angle}
void Scene::convertModel(
    std::vector<Vertex> &result,
    const std::vector<Vertex> &vertices,
    const std::optional<CoordinateVector> &moveConvert)
{
    result.clear();
    result.reserve(vertices.size());

    auto convertMatrix =
        Matrix<4, 4>::getProjectionConvert(camera.getFOV(), camera.getAspect(), 2000, 0.1) *
        Matrix<4, 4>::getObserverConvert(camera.cGetPosition(), camera.cGetTarget(), up);

    if (moveConvert.has_value())
        convertMatrix = convertMatrix * Matrix<4, 4>::getMoveConvert(*moveConvert);

    static CoordinateVector cv;

    for (auto it = vertices.begin(); it < vertices.end(); ++it)
    {
        bool isOutOfScreen = false;
        bool isWNegative = false;

        cv = Converter::vertexToCVector(*it);

        cv = Converter::matrixToCVector(convertMatrix * Converter::cVectorToMatrix(cv));

        if (cv.cGetW() <= 0)
            isWNegative = true;

        cv = cv / cv.cGetW();

        if (cv.cGetX() < -1 || cv.cGetX() > 1 ||
            cv.cGetY() < -1 || cv.cGetY() > 1 ||
            cv.cGetZ() < 0 || cv.cGetZ() > 1)
            isOutOfScreen = true;

        cv = Converter::matrixToCVector(
            Matrix<4, 4>::getWindowConvert(camera.cGetResolution().x, camera.cGetResolution().y, 0, 0) * cv);

        result.emplace_back(Converter::cVectorToVertex(cv, isOutOfScreen, isWNegative));
    }
}

void Scene::centralizeCamera()
{
    camera.getTarget() = Converter::vertexToCVector(objects.at(selectedObjectName)->getCenter()) + objectsShift.at(selectedObjectName);
}

void Scene::rotateCamera(const AxisName axisName, const double angle)
{
    auto rConvert = Matrix<4, 4>::getRotateConvert(axisName, angle);

    camera.getTarget() = Converter::matrixToCVector(rConvert * camera.cGetTarget());
}

void Scene::rotateCameraAround(
    const AxisName axisName,
    const Direction direction,
    const int dt)
{
    const double ratio = dt != 0 ? ((double)dt / defaultFrameTime) : 1.f;
    const double rotationSpeedTimed = rotationSpeed * ratio;

    auto cameraRelative = Converter::matrixToCVector(camera.cGetPosition() - camera.cGetTarget());
    auto spherical = Math::decartToSpherical(cameraRelative);

    bool isCameraReversed = false;
    spherical.move(axisName, direction, rotationSpeedTimed, isCameraReversed);

    if (isCameraReversed)
        up.getY() = -up.cGetY();

    cameraRelative = Math::sphericalToDecart(spherical);
    camera.getPosition() = cameraRelative + camera.cGetTarget();
}

void Scene::moveCamera(const CoordinateVector &transition)
{
    camera.getTarget() = camera.cGetTarget() + transition;
    camera.getPosition() = camera.cGetPosition() + transition;
}

ObjInfo *Scene::getObject(const std::string key)
{
    return objects.at(key);
}

void Scene::addObject(const std::string key, ObjInfo *object)
{
    if (key == floorObjectName)
        throw std::invalid_argument("This object name is reserved!");

    selectedObjectName = key;

    objects[key] = object;

    objectsConvertedVertices[key] = std::vector<Vertex>(
        object->cGetVertices().size());

    objectsShift[key] = CoordinateVector(0, 0, 0, 1);

    generateFloor();
}

CoordinateVector &Scene::getObjectShift(const std::string key)
{
    return objectsShift.at(key);
}

std::vector<Vertex> &Scene::getObjectConvertedVertices(const std::string key)
{
    return objectsConvertedVertices.at(key);
}

const std::vector<std::string> Scene::getAllObjectNames() const
{
    auto res = std::vector<std::string>();

    for (auto pair : objects)
        res.emplace_back(pair.first);

    return res;
}

const std::string Scene::getSelectedObjectName() const
{
    return selectedObjectName;
}

CoordinateVector Scene::getMoveConvert(
    const AxisName axis,
    const Direction direction,
    const int dt)
{
    CoordinateVector transition;

    const double ratio = dt != 0 ? ((double)dt / defaultFrameTime) : 1.f;
    const double moveSpeedTimed = moveSpeed * ratio;

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

const Camera &Scene::cGetCamera() const
{
    return camera;
}

Camera &Scene::getCamera()
{
    return camera;
}
