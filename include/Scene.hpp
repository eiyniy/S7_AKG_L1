#pragma once

#include <ObjInfo.hpp>
#include <Camera.hpp>
#include <CoordinateVector.hpp>
#include <Types.hpp>

class Scene
{
private:
    const ObjInfo &cObjInfo;
    const CoordinateVector &up;
    const int moveSpeed;

    ObjInfo objInfo;
    Camera &camera;

public:
    const int defaultFps = 60;
    const int defaultFrameTime = (1.f / defaultFps) * 1000;

    Scene(ObjInfo &p_objInfo, Camera &p_camera, const CoordinateVector &p_up, const int p_moveSpeed);

    void modelConvert();

    void moveConvert(AxisName axis, Direction direction, int dt);
    void scaleConvert();
    void rotateConvert();

    ObjInfo &getObjInfoCopy();
    const ObjInfo &cGetObjInfo() const;
    const Camera &cGetCamera() const;

    Camera &getCamera();
};
