#include <iostream>
#include <Scene.hpp>

Scene::Scene(ObjInfo &p_objInfo, Camera &p_camera, const CoordinateVector &p_up)
    : objInfo(p_objInfo), camera(p_camera), up(p_up) {}

void Scene::modelConvert()
{
    std::vector<Vertex> &vertices = objInfo.getVertices();

    for (auto it = vertices.begin(); it < vertices.end(); ++it)
    {
        auto cv = CoordinateVector(it->getX(), it->getY(), it->getZ(), it->getW().value_or(1));

        // cv.log();
        cv.toObserverConvert(camera.getPosition(), camera.getTarget(), up);
        // cv.log();
        cv.toProjectionConvert(camera.getFOV(), camera.getAspect(), 1000000, 0);
        // cv.log();
        cv.toViewerConvert(camera.cGetResolution().x, camera.cGetResolution().y, 0, 0);
        // cv.log();

        *it = Vertex(BaseVertex::fromMatrix(cv));

        // std::cout << "it: " << it->getX() << " " << it->getY() << " " << it->getZ() << " " << it->getW().value_or(1) << std::endl;
    }

    std::cout << "after it: "
              << objInfo.cGetVertices().begin()->getX() << " "
              << objInfo.cGetVertices().begin()->getY() << " "
              << objInfo.cGetVertices().begin()->getZ() << " "
              << objInfo.cGetVertices().begin()->getW().value_or(1)
              << std::endl;
}

const ObjInfo &Scene::cGetObjInfo() const
{
    return objInfo;
}

const Camera &Scene::cGetCamera() const
{
    return camera;
}

Camera &Scene::getCamera()
{
    return camera;
}
