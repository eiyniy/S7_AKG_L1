#include <cmath>
#include <memory>
#include <Object.hpp>
#include <Converter.hpp>
#include <ThreadPool.hpp>
#include <Timer.hpp>
#include <Globals.hpp>

DrawableVertex convertVertex(
    const Matrix<4, 1> &,
    const Matrix<4, 4> &,
    const Matrix<4, 4> &);

Object::Object(
    const std::vector<Matrix<4, 1>> &_vertices,
    const std::vector<Matrix<4, 1>> &_tVertices,
    const std::vector<Matrix<4, 1>> &_nVertices,
    const std::vector<Triangle> &_polygons,
    const std::optional<Texture<sf::Color>> &_diffuseMap,
    const std::optional<Texture<Matrix<4, 1>>> &_normalMap,
    const std::optional<Texture<Matrix<4, 1>>> &_mraoMap,
    const std::optional<Texture<sf::Color>> &_emissiveMap)
    : vertices(_vertices),
      tVertices(_tVertices),
      nVertices(_nVertices),
      polygons(_polygons),
      diffuseMap(_diffuseMap),
      normalMap(_normalMap),
      mraoMap(_mraoMap),
      emissiveMap(_emissiveMap)
{
    drawable = std::vector<DrawableVertex>(vertices.size());
}

void Object::move(const Matrix<4, 1> &transition)
{
    const auto moveConvert = Matrix<4, 4>::getMoveConvert(transition);

    // vertices[0].log();

#pragma omp parallel for if (!_DEBUG)
    for (int j = 0; j < vertices.size(); ++j)
        vertices[j] = moveConvert * vertices[j];

    // vertices[0].log();
    std::cout << std::endl;
}

void Object::convertToDrawable(const Camera &camera)
{
    const auto convertMatrix =
        Matrix<4, 4>::getProjectionConvert(camera.cGetFOV(), camera.cGetAspect(), 2000, 0.1) *
        Matrix<4, 4>::getViewConvert(camera.cGetPosition(), camera.cGetTarget(), camera.cGetUp());

    const auto &resolution = camera.cGetResolution();
    const auto viewportConvert = Matrix<4, 4>::getViewportConvert(
        resolution.cGetX(), resolution.cGetY(), 0, 0);

    // /*
#pragma omp parallel for if (!_DEBUG)
    for (int j = 0; j < vertices.size(); ++j)
        drawable[j] = convertVertex(vertices[j], convertMatrix, viewportConvert);
    // */
}

void Object::calcGeometricParams()
{
    double cx = 0, cy = 0, cz = 0;

    auto first = vertices.cbegin();
    double xMax = first->cGetX(),
           xMin = first->cGetX(),
           zMax = first->cGetZ(),
           zMin = first->cGetZ();

    for (auto &&vertex : vertices)
    {
        const auto x = vertex.cGetX();
        const auto y = vertex.cGetY();
        const auto z = vertex.cGetZ();

        if (x > xMax)
            xMax = x;
        else if (x < xMin)
            xMin = x;
        if (z > zMax)
            zMax = z;
        else if (z < zMin)
            zMin = z;

        cx += x;
        cy += y;
        cz += z;
    }

    const auto vCount = (double)vertices.size();

    center = Matrix<4, 1>(cx / vCount, cy / vCount, cz / vCount);
    maxXZ = Matrix<4, 1>(xMax, 0, zMax);
    minXZ = Matrix<4, 1>(xMin, 0, zMin);
}

const Matrix<4, 1> &Object::getCenter()
{
    // if (!center.has_value())
    calcGeometricParams();

    return *center;
}

const Matrix<4, 1> &Object::getMaxXZ()
{
    // if (!maxXZ.has_value())
    calcGeometricParams();

    return *maxXZ;
}

const Matrix<4, 1> &Object::getMinXZ()
{
    // if (!minXZ.has_value())
    calcGeometricParams();

    return *minXZ;
}

DrawableVertex convertVertex(
    const Matrix<4, 1> &vertex,
    const Matrix<4, 4> &toProjectionConvert,
    const Matrix<4, 4> &viewportConvert)
{
    bool isWNegative = false;

    auto vertexCopy = toProjectionConvert * vertex;

    if (vertexCopy.cGetW() <= 0)
        isWNegative = true;

    const auto w = vertexCopy.cGetW();
    vertexCopy /= w;

    // if (mVertex.cGetX() < -1 || mVertex.cGetX() > 1 ||
    //     mVertex.cGetY() < -1 || mVertex.cGetY() > 1 ||
    //     mVertex.cGetZ() < 0 || mVertex.cGetZ() > 1)
    //     isVisible = false;

    vertexCopy = viewportConvert * vertexCopy;

    return Converter::matrixToDrawableVertex(vertexCopy, w, isWNegative);
}
