#pragma once

#include <ObjInfo.hpp>
#include <Camera.hpp>
#include <Matrix.hpp>
#include <Enums.hpp>
#include <SFML/Graphics.hpp>
#include <optional>

class Scene
{
private:
    const int floorStepsCount = 30;

    std::string selectedObjectName;
    std::map<std::string, ObjInfo *> objects;
    std::map<std::string, Matrix<4, 1>> objectsShift;
    std::map<std::string, std::vector<Vertex>> objectsConvertedVertices;
    Camera &camera;
    Matrix<4, 1> &up;

    void generateFloor();
    void generateFloor(const int size, const int step, const Point &center);

public:
    const std::string floorObjectName = "_FLOOR";

    const double defaultFrameTime;
    const double moveSpeed;
    const double rotationSpeed;

    Scene(
        Camera &_camera,
        Matrix<4, 1> &_up,
        const double _moveSpeed,
        const double _rotationSpeed);

    ~Scene();

    const Matrix<4, 1> cGetTransition(
        const AxisName axis,
        const Direction direction,
        const int dt) const;

    void convertAllModels();
    void convertModel(
        std::vector<Vertex> &result,
        const std::vector<Vertex> &vertices,
        const Matrix<4, 1> &objectShift);

    void flip();
    void centralizeCamera();
    void moveObject(const std::string &objectName, const Matrix<4, 1> &transition);
    void addObject(const std::string &key, ObjInfo *object);

    const ObjInfo *cGetObject(const std::string &key) const;
    const Matrix<4, 1> &cGetObjectShift(const std::string &key) const;
    const std::vector<Vertex> &cGetObjectConvertedVertices(const std::string &key) const;
    const std::vector<std::string> cGetAllObjectNames() const;
    const std::string cGetSelectedObjectName() const;
    const Camera &cGetCamera() const;

    Matrix<4, 1> &getObjectShift(const std::string &key);
    Camera &getCamera();
};

inline const ObjInfo *Scene::cGetObject(const std::string &key) const
{
    return objects.at(key);
}

inline const Matrix<4, 1> &Scene::cGetObjectShift(const std::string &key) const
{
    return objectsShift.at(key);
}

inline Matrix<4, 1> &Scene::getObjectShift(const std::string &key)
{
    return objectsShift.at(key);
}

inline const std::vector<Vertex> &Scene::cGetObjectConvertedVertices(const std::string &key) const
{
    return objectsConvertedVertices.at(key);
}
