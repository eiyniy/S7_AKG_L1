#pragma once

#include <ObjInfo.hpp>
#include <Camera.hpp>
#include <CoordinateVector.hpp>

class Scene
{
private:
    ObjInfo &objInfo;
    const CoordinateVector &up;
    Camera &camera;

public:
    Scene(ObjInfo &p_objInfo, Camera &p_camera, const CoordinateVector &p_up);

    void modelConvert();

    const ObjInfo &cGetObjInfo() const;
    const Camera &cGetCamera() const;

    Camera &getCamera();
};
