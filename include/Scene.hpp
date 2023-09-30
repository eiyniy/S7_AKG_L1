#pragma once

#include <ObjInfo.hpp>
#include <Camera.hpp>
#include <CoordinateVector.hpp>

class Scene
{
private:
    const ObjInfo &objInfo;
    const Camera &camera;
    const CoordinateVector &up;

public:
    Scene(const ObjInfo &p_objInfo, const Camera &p_camera, const CoordinateVector &p_up);

    void modelConvert() const;
    const ObjInfo &getObjInfo() const;
};
