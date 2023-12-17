#include <cmath>
#include <memory>
#include <Object.hpp>
#include <Converter.hpp>
#include <ThreadPool.hpp>
#include <Timer.hpp>

DrawableVertex convertVertex(
    const Matrix<4, 1> &,
    const Matrix<4, 4> &,
    const Matrix<4, 4> &);

Object::Object(
    const std::vector<Matrix<4, 1>> &_vertices,
    const std::vector<Matrix<4, 1>> &_tVertices,
    const std::vector<Matrix<4, 1>> &_nVertices,
    const std::vector<Polygon> &_polygons,
    const sf::Color &_color)
    : color(_color),
      vertices(_vertices),
      tVertices(_tVertices),
      nVertices(_nVertices),
      polygons(_polygons),
      shift({0, 0, 0, 1})
{
    drawable = std::vector<DrawableVertex>(vertices.size());
}

void Object::move(const Matrix<4, 1> &transition)
{
    shift += transition;
}

std::vector<DrawableVertex> Object::getDrawable(const Camera &camera)
{
    const auto convertMatrix =
        Matrix<4, 4>::getProjectionConvert(camera.cGetFOV(), camera.cGetAspect(), 2000, 0.1) *
        Matrix<4, 4>::getViewConvert(camera.cGetPosition(), camera.cGetTarget(), camera.cGetUp()) *
        Matrix<4, 4>::getMoveConvert(shift);

    const auto &resolution = camera.cGetResolution();
    const auto viewportConvert = Matrix<4, 4>::getViewportConvert(
        resolution.cGetX(), resolution.cGetY(), 0, 0);

    /*
    // const int threadsCount = (unsigned int)ceil(polygons.size() / 10000.f);
    const int threadsCount = (int)std::min(
        (unsigned int)std::ceil((float)vertices.size() / 10000.f),
        ThreadPool::getInstance().getThreadsCount());
    const double size = (double)vertices.size() / (double)threadsCount;

    for (int i = 0; i < threadsCount; ++i)
    {
        const int begin = (int)std::floor(size * i);
        const int end = (int)std::floor(size * (i + 1)) - 1;

        ThreadPool::getInstance().enqueue(
            [begin, end, &convertMatrix, &viewportConvert,
             thisDrawable = &this->drawable,
             thisVertices = &this->vertices]()
            {
                for (int j = begin; j <= end; ++j)
                    thisDrawable->at(j) = convertVertex(
                        thisVertices->at(j), convertMatrix, viewportConvert);
            });
    }

    ThreadPool::getInstance().waitAll();
    */

    // /*
    for (int j = 0; j < vertices.size(); ++j)
        drawable[j] = convertVertex(vertices[j], convertMatrix, viewportConvert);
    // */

    return drawable;
}

const std::vector<Matrix<4, 1>> &Object::cGetVertices() const
{
    return vertices;
}

const std::vector<Matrix<4, 1>> &Object::cGetTVertices() const
{
    return tVertices;
}

const std::vector<Matrix<4, 1>> &Object::cGetNVertices() const
{
    return nVertices;
}

const std::vector<Polygon> &Object::cGetPolygons() const
{
    return polygons;
}

std::vector<Polygon> &Object::getPolygons()
{
    return polygons;
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
    if (!center.has_value())
        calcGeometricParams();

    return *center;
}

const Matrix<4, 1> &Object::getMaxXZ()
{
    if (!maxXZ.has_value())
        calcGeometricParams();

    return *maxXZ;
}

const Matrix<4, 1> &Object::getMinXZ()
{
    if (!minXZ.has_value())
        calcGeometricParams();

    return *minXZ;
}

DrawableVertex convertVertex(
    const Matrix<4, 1> &vertex,
    const Matrix<4, 4> &toProjectionConvert,
    const Matrix<4, 4> &viewportConvert)
{
    bool isVisible = true;
    bool isWNegative = false;

    auto vertexCopy = toProjectionConvert * vertex;

    if (vertexCopy.cGetW() <= 0)
    {
        isVisible = false;
        isWNegative = true;
    }

    const auto w = vertexCopy.cGetW();
    vertexCopy /= w;

    // if (mVertex.cGetX() < -1 || mVertex.cGetX() > 1 ||
    //     mVertex.cGetY() < -1 || mVertex.cGetY() > 1 ||
    //     mVertex.cGetZ() < 0 || mVertex.cGetZ() > 1)
    //     isVisible = false;

    vertexCopy = viewportConvert * vertexCopy;

    return Converter::matrixToDrawableVertex(vertexCopy, w, isVisible, isWNegative);
}
