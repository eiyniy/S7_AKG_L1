#pragma once

#include <ObjInfo.hpp>
#include <Camera.hpp>
#include <CoordinateVector.hpp>

class Scene
{
private:
    ObjInfo &objInfo;
    Camera &camera;
    CoordinateVector &up;

public:
    Scene(ObjInfo &p_objInfo, Camera &p_camera, CoordinateVector &p_up);

    void modelConvert();
    ObjInfo &getObjInfo();
};
