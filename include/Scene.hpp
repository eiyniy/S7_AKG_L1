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
    const double moveSpeed;

    std::vector<Vertex> floor;
    std::vector<Vertex> objInfoVertices;
    CoordinateVector worldShift;
    Camera &camera;

public:
    const int defaultFps = 60;
    const int defaultFrameTime = (1.f / defaultFps) * 1000;

    Scene(ObjInfo &p_objInfo, Camera &p_camera, const CoordinateVector &p_up, const double p_moveSpeed);

    CoordinateVector getMoveConvert(AxisName axis, Direction direction, int dt);

    void modelConvert(const std::vector<Vertex> &vertices, const std::optional<CoordinateVector> &moveConvert = std::nullopt);
    void moveCamera(CoordinateVector &transition);

    const std::vector<Vertex> &cGetObjInfoVertices() const;
    const Camera &cGetCamera() const;
    const CoordinateVector &cGetWorldShift() const;
    const std::vector<Vertex> &cGetObjInfoVerticesCopy() const;

    std::vector<Vertex> &getFloorCopy();

    Camera &getCamera();
    CoordinateVector &getWorldShift();

    std::vector<std::array<sf::Vertex, 2>> getDrawableFloor();
};
