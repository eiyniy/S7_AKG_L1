#pragma once

#include <Object.hpp>
#include <Camera.hpp>
#include <Matrix.hpp>
#include <Enums.hpp>
#include <SFML/Graphics.hpp>
#include <optional>
#include <BaseLightSource.hpp>

class Scene {
private:
    const int floorStepsCount = 30;

    std::string selectedObjectName;
    std::map<std::string, Object *> objects;
    Camera &camera;
    BaseLightSource *lightSource;

    void generateFloor();

    void generateFloor(const int size, const int step, const Point &center);

public:
    const std::string floorObjectName = "_FLOOR";

    const double defaultFrameTime;
    const double moveSpeed;
    const double rotationSpeed;

    Scene(
            Camera &_camera,
            BaseLightSource *_lightSource,
            const double _moveSpeed,
            const double _rotationSpeed);

    ~Scene();

    void centralizeCamera();

    void addObject(const std::string &key, Object *object);

    const Object *cGetObject(const std::string &key) const;

    const std::vector<std::string> cGetAllObjectNames() const;

    const std::string cGetSelectedObjectName() const;

    const Camera &cGetCamera() const;

    const BaseLightSource *cGetLightSource() const;

    Object *getObject(const std::string &key);

    Camera &getCamera();
};

inline const Object *Scene::cGetObject(const std::string &key) const {
    return objects.at(key);
}

inline const Camera &Scene::cGetCamera() const {
    return camera;
}

inline Object *Scene::getObject(const std::string &key) {
    return objects.at(key);
}

inline Camera &Scene::getCamera() {
    return camera;
}
