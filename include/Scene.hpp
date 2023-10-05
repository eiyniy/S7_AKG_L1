#pragma once

#include <ObjInfo.hpp>
#include <Camera.hpp>
#include <CoordinateVector.hpp>
#include <Types.hpp>
#include <SFML/Graphics.hpp>

class Scene
{
private:
    const ObjInfo &cObjInfo;
    const CoordinateVector &up;
    const int moveSpeed;

    std::vector<CoordinateVector> floor;
    CoordinateVector worldShift;
    ObjInfo objInfo;
    Camera &camera;

public:
    const int defaultFps = 60;
    const int defaultFrameTime = (1.f / defaultFps) * 1000;

    Scene(ObjInfo &p_objInfo, Camera &p_camera, const CoordinateVector &p_up, const int p_moveSpeed);

    void modelConvert(std::vector<Vertex> &vertices);

    void moveConvert(AxisName axis, Direction direction, int dt);
    void scaleConvert();
    void rotateConvert();

    const ObjInfo &cGetObjInfo() const;
    const Camera &cGetCamera() const;

    ObjInfo &getObjInfoCopy();
    Camera &getCamera();

    std::vector<std::array<sf::Vertex, 2>> getFloor();
};
