#include <iostream>
#include <future>
#include <thread>
#include <Scene.hpp>
#include <Converter.hpp>
#include <Timer.hpp>
#include <Math.hpp>

Scene::Scene(
    Camera &p_camera,
    Matrix<4, 1> &p_up,
    const double p_moveSpeed,
    const double p_rotationSpeed)
    : camera(p_camera),
      up(p_up),
      moveSpeed(p_moveSpeed),
      rotationSpeed(p_rotationSpeed),
      isMoveChanged(true),
      isRotateChanged(true),
      isScaleChanged(true),
      isObserverChanged(true),
      isProjectionChanged(true),
      isWindowChanged(true)
{
    generateFloor(25, 20, Dot(0, 0));
    convertAllModels();
}

Scene::~Scene()
{
    for (auto pair : objects)
        delete pair.second;
}

void Scene::generateFloor()
{
    double maxDimensionSize = 0;
    auto center = Dot(0, 0);

    for (auto pair : objects)
    {
        if (pair.first == floorObjectName)
            continue;

        auto maxXZ = Converter::vertexToMatrix(pair.second->getMaxXZ());
        auto minXZ = Converter::vertexToMatrix(pair.second->getMinXZ());

        auto floorSize = maxXZ - minXZ;

        auto biggerDimensionSize = std::max(floorSize.cGetX(), floorSize.cGetZ());

        if (biggerDimensionSize > maxDimensionSize)
        {
            maxDimensionSize = biggerDimensionSize;
            center = Converter::vertexToDot(pair.second->getCenter());
        }
    }

    maxDimensionSize *= 3;

    auto step = maxDimensionSize / floorStepsCount;
    generateFloor(floorStepsCount, ceil(step), center);
}

void Scene::generateFloor(const int size, const int step, const Dot &center)
{
    auto color = sf::Color(255U, 255U, 255U, 64U);
    auto floorPt = new ObjInfo(color);

    const auto evenSize = size % 2 == 0 ? size + 1 : size;
    const auto halfSize = evenSize / 2;

    for (int i = -halfSize; i <= halfSize; ++i)
    {
        for (int j = -halfSize; j <= halfSize; ++j)
            floorPt->addVertex(Vertex(j * step + center.x, 0, i * step + center.y));
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

    objects.insert_or_assign(floorObjectName, floorPt);

    objectsConvertedVertices.insert_or_assign(
        floorObjectName,
        std::vector<Vertex>(floorPt->cGetVertices().size()));

    objectsShift.insert_or_assign(
        floorObjectName,
        Matrix<4, 1>(0, 0, 0));
}

void Scene::convertAllModels()
{
    if (isObserverChanged)
    {
        observerConvertCached = Matrix<4, 4>::getObserverConvert(camera.cGetPosition(), camera.cGetTarget(), up);
        isObserverChanged = false;
    }
    if (isProjectionChanged)
    {
        projectionConvertCached = Matrix<4, 4>::getProjectionConvert(camera.getFOV(), camera.getAspect(), 2000, 0.1);
        isProjectionChanged = false;
    }
    if (isWindowChanged)
    {
        windowConvertCached = Matrix<4, 4>::getWindowConvert(camera.cGetResolution().x, camera.cGetResolution().y, 0, 0);
        isWindowChanged = false;
    }

    for (auto pair : objects)
    {
        convertModel(
            objectsConvertedVertices.at(pair.first),
            objects.at(pair.first)->cGetVertices(),
            objectsShift.at(pair.first));
    }
}

// TODO: Replace rotateConvert with RotateInfo type {AxisName & angle}
void Scene::convertModel(
    std::vector<Vertex> &result,
    const std::vector<Vertex> &vertices,
    const Matrix<4, 1> &objectShift)
{
    if (isMoveChanged)
    {
        moveConvertCached = Matrix<4, 4>::getMoveConvert(objectShift);
        isMoveChanged = false;
    }

    const auto convertMatrix = projectionConvertCached * observerConvertCached * moveConvertCached;

    int i = 0;
    for (auto it = vertices.begin(); it < vertices.cend(); ++it, ++i)
    {
        bool isOutOfScreen = false;

        Matrix<4, 1> mVertex = Converter::vertexToMatrix(*it);

        mVertex = convertMatrix * mVertex;

        if (mVertex.cGetW() <= 0)
            isOutOfScreen = true;

        mVertex /= mVertex.cGetW();

        if (mVertex.cGetX() < -1 || mVertex.cGetX() > 1 ||
            mVertex.cGetY() < -1 || mVertex.cGetY() > 1 ||
            mVertex.cGetZ() < 0 || mVertex.cGetZ() > 1)
            isOutOfScreen = true;

        mVertex = windowConvertCached * mVertex;

        result[i] = Converter::matrixToVertex(mVertex, isOutOfScreen);
    }
}

void Scene::centralizeCamera()
{
    camera.getTarget() =
        Converter::vertexToMatrix(objects.at(selectedObjectName)->getCenter()) +
        objectsShift.at(selectedObjectName);

    isObserverChanged = true;
}

void Scene::rotateCameraAround(
    const AxisName axisName,
    const Direction direction,
    const int dt)
{
    const double ratio = dt != 0 ? ((double)dt / defaultFrameTime) : 1.f;
    const double rotationSpeedTimed = rotationSpeed * ratio;

    auto cameraRelative = camera.cGetPosition() - camera.cGetTarget();
    auto spherical = Math::decartToSpherical(cameraRelative);

    bool isCameraReversed = false;
    spherical.move(axisName, direction, rotationSpeedTimed, isCameraReversed);

    if (isCameraReversed)
        up.getY() = -up.cGetY();

    cameraRelative = Math::sphericalToDecart(spherical);
    camera.getPosition() = cameraRelative + camera.cGetTarget();

    isObserverChanged = true;
}

void Scene::moveCamera(const Matrix<4, 1> &transition)
{
    camera.getTarget() = camera.cGetTarget() + transition;
    camera.getPosition() = camera.cGetPosition() + transition;

    isObserverChanged = true;
}

void Scene::moveObject(const std::string &objectName, const Matrix<4, 1> &transition)
{
    getObjectShift(objectName) += transition;

    isMoveChanged = true;
}

void Scene::resize(const int width, const int height)
{
    camera.getResolution() = Dot(width, height);

    isProjectionChanged = true;
    isWindowChanged = true;

    convertAllModels();
}

ObjInfo *Scene::getObject(const std::string &key)
{
    return objects.at(key);
}

void Scene::addObject(const std::string &key, ObjInfo *object)
{
    if (key == floorObjectName)
        throw std::invalid_argument("This object name is reserved!");

    selectedObjectName = key;

    objects[key] = object;

    objectsConvertedVertices[key] = std::vector<Vertex>(
        object->cGetVertices().size());

    objectsShift[key] = Matrix<4, 1>(0, 0, 0, 1);

    generateFloor();
}

Matrix<4, 1> &Scene::getObjectShift(const std::string &key)
{
    return objectsShift.at(key);
}

std::vector<Vertex> &Scene::getObjectConvertedVertices(const std::string &key)
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

Matrix<4, 1> Scene::getTransition(
    const AxisName axis,
    const Direction direction,
    const int dt)
{
    Matrix<4, 1> transition;

    const double ratio = dt != 0 ? ((double)dt / defaultFrameTime) : 1.f;
    const double moveSpeedTimed = moveSpeed * ratio;

    switch (axis)
    {
    case AxisName::X:
        if (direction == Direction::Forward)
            transition = Matrix<4, 1>(moveSpeedTimed, 0, 0, 0);
        else
            transition = Matrix<4, 1>(-moveSpeedTimed, 0, 0, 0);

        break;
    case AxisName::Y:
        if (direction == Direction::Forward)
            transition = Matrix<4, 1>(0, moveSpeedTimed, 0, 0);
        else
            transition = Matrix<4, 1>(0, -moveSpeedTimed, 0, 0);

        break;
    case AxisName::Z:
        if (direction == Direction::Forward)
            transition = Matrix<4, 1>(0, 0, moveSpeedTimed, 0);
        else
            transition = Matrix<4, 1>(0, 0, -moveSpeedTimed, 0);

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
