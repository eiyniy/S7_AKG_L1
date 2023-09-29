#include <iostream>
#include <Scene.hpp>

Scene::Scene(ObjInfo &p_objInfo, Camera &p_camera, CoordinateVector &p_up)
    : objInfo(p_objInfo), camera(p_camera), up(p_up) {}

void Scene::modelConvert()
{
    auto vertices = objInfo.getVertices();

    for (auto &el : vertices)
    {
        auto cv = CoordinateVector::fromCoordinats(el.getX(), el.getY(), el.getZ(), el.getW().value_or(1));

        cv.toObserverConvert(camera.getPosition(), camera.getTarget(), up);
        cv.toProjectionConvert(80, 640 / 480, 1, 0);
        cv.toViewerConvert(640, 480, 0, 0);

        auto bv = BaseVertex::fromMatrix(cv);
        el = *static_cast<Vertex *>(&bv);
    }
}

ObjInfo &Scene::getObjInfo()
{
    return objInfo;
}
