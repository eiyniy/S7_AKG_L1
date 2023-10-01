#pragma once

#include <ObjInfo.hpp>
#include <Camera.hpp>
#include <CoordinateVector.hpp>
#include <Types.hpp>

class Scene
{
private:
    ObjInfo &objInfo;
    const CoordinateVector &up;
    Camera &camera;

public:
    Scene(ObjInfo &p_objInfo, Camera &p_camera, const CoordinateVector &p_up);

    void modelConvert();

    void moveConvert(AxisName axis, Direction direction);
    void scaleConvert();
    void rotateConvert();

    const ObjInfo &cGetObjInfo() const;
    const Camera &cGetCamera() const;

    Camera &getCamera();
};
