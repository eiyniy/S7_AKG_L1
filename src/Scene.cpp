#include <iostream>
#include <Scene.hpp>

Scene::Scene(const ObjInfo &p_objInfo, const Camera &p_camera, const CoordinateVector &p_up)
    : objInfo(p_objInfo), camera(p_camera), up(p_up) {}

void Scene::modelConvert() const
{
    auto vertices = objInfo.getVertices();

    for (auto &el : vertices)
    {
        auto cv = CoordinateVector(el.getX(), el.getY(), el.getZ(), el.getW().value_or(1));

        cv.toObserverConvert(camera.getPosition(), camera.getTarget(), up);
        // cv.log();
        cv.toProjectionConvert(80, 640 / 480, 1, 0);
        // cv.log();
        cv.toViewerConvert(640, 480, 0, 0);
        // cv.log();

        el = Vertex(BaseVertex::fromMatrix(cv));
    }
}

const ObjInfo &Scene::getObjInfo() const
{
    return objInfo;
}
