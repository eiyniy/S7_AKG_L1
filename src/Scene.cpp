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
        cv.toProjectionConvert(camera.getFOV(), camera.getAspect(), 1, 0);
        // cv.log();
        cv.toViewerConvert(camera.getResolution().x, camera.getResolution().y, 0, 0);
        // cv.log();

        el = Vertex(BaseVertex::fromMatrix(cv));
    }
}

const ObjInfo &Scene::getObjInfo() const
{
    return objInfo;
}

const Camera &Scene::getCamera() const
{
    return camera;
}
