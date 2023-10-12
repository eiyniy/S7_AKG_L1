#pragma once

#include <ObjInfo.hpp>
#include <Camera.hpp>
#include <Matrix.hpp>
#include <Types.hpp>
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

    const double moveSpeed;
    const double rotationSpeed;

    Matrix<4, 1> &up;

    bool isMoveChanged;
    bool isRotateChanged;
    bool isScaleChanged;
    bool isObserverChanged;
    bool isProjectionChanged;
    bool isWindowChanged;

    Matrix<4, 4> moveConvertCached;
    Matrix<4, 4> rotateConvertCached;
    Matrix<4, 4> scaleConvertCached;
    Matrix<4, 4> observerConvertCached;
    Matrix<4, 4> projectionConvertCached;
    Matrix<4, 4> windowConvertCached;

    void generateFloor();
    void generateFloor(const int size, const int step, const Dot &center);

public:
    const std::string floorObjectName = "_FLOOR";

    const int defaultFps = 165;
    const double defaultFrameTime = (1.f * 1000.f / 60);

    Scene(
        Camera &p_camera,
        Matrix<4, 1> &p_up,
        const double p_moveSpeed,
        const double p_rotationSpeed);

    ~Scene();

    Matrix<4, 1> getTransition(
        const AxisName axis,
        const Direction direction,
        const int dt);

    void convertAllModels();
    void convertModel(
        std::vector<Vertex> &result,
        const std::vector<Vertex> &vertices,
        const Matrix<4, 1> &objectShift);

    void centralizeCamera();
    void rotateCameraAround(
        const AxisName axisName,
        const Direction direction,
        const int dt);
    void moveCamera(const Matrix<4, 1> &transition);

    void moveObject(const std::string &objectName, const Matrix<4, 1> &transition);

    void resize(const int width, const int height);

    ObjInfo *getObject(const std::string &key);
    void addObject(const std::string &key, ObjInfo *object);

    Matrix<4, 1> &getObjectShift(const std::string &key);
    std::vector<Vertex> &getObjectConvertedVertices(const std::string &key);

    const std::vector<std::string> getAllObjectNames() const;
    const std::string getSelectedObjectName() const;

    const Camera &cGetCamera() const;
    Camera &getCamera();
};
