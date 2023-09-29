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
    CoordinateVector &target;

public:
    Scene(ObjInfo &p_objInfo, Camera &p_camera, CoordinateVector &p_up, CoordinateVector &p_target);

    void modelConvert();
    ObjInfo &getObjInfo();
};
