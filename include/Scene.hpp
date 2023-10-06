#pragma once

#include <ObjInfo.hpp>
#include <Camera.hpp>
#include <CoordinateVector.hpp>
#include <Types.hpp>
#include <SFML/Graphics.hpp>
#include <optional>

class Scene
{
private:
    const ObjInfo &cObjInfo;
    std::vector<Vertex> cFloor;
    const CoordinateVector &up;
    const int moveSpeed;

    std::vector<Vertex> floor;
    CoordinateVector worldShift;
    ObjInfo objInfo;
    Camera &camera;

public:
    const int defaultFps = 60;
    const int defaultFrameTime = (1.f / defaultFps) * 1000;

    Scene(ObjInfo &p_objInfo, Camera &p_camera, const CoordinateVector &p_up, const int p_moveSpeed);

    CoordinateVector getMoveConvert(AxisName axis, Direction direction, int dt);

    void modelConvert(std::vector<Vertex> &vertices, std::optional<CoordinateVector> moveConvert = std::nullopt);

    const ObjInfo &cGetObjInfo() const;
    const Camera &cGetCamera() const;

    ObjInfo &getObjInfoCopy();
    std::vector<Vertex> &getFloorCopy();
    Camera &getCamera();

    std::vector<std::array<sf::Vertex, 2>> getDrawableFloor();
};
