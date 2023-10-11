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
    const int floorStepsCount = 20;

    std::string selectedObjectName;
    std::map<std::string, ObjInfo *> objects;
    std::map<std::string, Matrix<4, 1>> objectsShift;
    std::map<std::string, std::vector<Vertex>> objectsConvertedVertices;
    Camera &camera;

    const double moveSpeed;
    const double rotationSpeed;

    Matrix<4, 1> &up;

    void generateFloor();
    void generateFloor(const int size, const int step);

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

    Matrix<4, 1> getMoveConvert(
        const AxisName axis,
        const Direction direction,
        const int dt);

    void convertAllModels();
    void convertModel(
        std::vector<Vertex> &result,
        const std::vector<Vertex> &vertices,
        const std::optional<Matrix<4, 1>> &moveConvert = std::nullopt);

    void centralizeCamera();
    void rotateCamera(const AxisName axisName, const double angle);
    void rotateCameraAround(
        const AxisName axisName,
        const Direction direction,
        const int dt);
    void moveCamera(const Matrix<4, 1> &transition);

    ObjInfo *getObject(const std::string key);
    void addObject(const std::string key, ObjInfo *object);

    Matrix<4, 1> &getObjectShift(const std::string key);
    std::vector<Vertex> &getObjectConvertedVertices(const std::string key);

    const std::vector<std::string> getAllObjectNames() const;
    const std::string getSelectedObjectName() const;

    const Camera &cGetCamera() const;
    Camera &getCamera();
};
