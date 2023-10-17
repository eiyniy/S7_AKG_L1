#include <optional>
#include <Object.hpp>
#include <Converter.hpp>

Object::Object(
    const std::vector<Vertex> &_vertices,
    const std::vector<TextureVertex> &_tVertices,
    const std::vector<NormalVertex> &_nVertices,
    const std::vector<Polygon> &_polygons,
    const sf::Color &_color)
    : color(_color),
      vertices(_vertices),
      tVertices(_tVertices),
      nVertices(_nVertices),
      polygons(_polygons),
      shift({0, 0, 0, 1}) {}

void Object::move(const Matrix<4, 1> &transition)
{
    shift += transition;
}

const std::vector<Vertex> Object::cGetDrawable(const Camera &camera) const
{
    auto result = std::vector<Vertex>(vertices.size());

    const auto convertMatrix =
        Matrix<4, 4>::getProjectionConvert(camera.cGetFOV(), camera.cGetAspect(), 2000, 0.1) *
        Matrix<4, 4>::getViewConvert(camera.cGetPosition(), camera.cGetTarget(), camera.cGetUp()) *
        Matrix<4, 4>::getMoveConvert(shift);

    int i = 0;
    static Matrix<4, 1> mVertex;
    for (auto it = vertices.begin(); it < vertices.cend(); ++it, ++i)
    {
        bool isOutOfScreen = false;

        mVertex = Converter::vertexToMatrix(*it);

        mVertex = convertMatrix * mVertex;

        if (mVertex.cGetW() <= 0)
            isOutOfScreen = true;

        mVertex /= mVertex.cGetW();

        if (mVertex.cGetX() < -1 || mVertex.cGetX() > 1 ||
            mVertex.cGetY() < -1 || mVertex.cGetY() > 1 ||
            mVertex.cGetZ() < 0 || mVertex.cGetZ() > 1)
            isOutOfScreen = true;

        mVertex = Matrix<4, 4>::getViewportConvert(camera.cGetResolution().cGetX(), camera.cGetResolution().cGetY(), 0, 0) * mVertex;

        result[i] = Converter::matrixToVertex(mVertex, isOutOfScreen);
    }

    return result;
}

const std::vector<Vertex> &Object::cGetVertices() const
{
    return vertices;
}

const std::vector<TextureVertex> &Object::cGetTVertices() const
{
    return tVertices;
}

const std::vector<NormalVertex> &Object::cGetNVertices() const
{
    return nVertices;
}

const std::vector<Polygon> &Object::cGetPolygons() const
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

    const auto vCount = vertices.size();

    center = Vertex(cx / vCount, cy / vCount, cz / vCount);
    maxXZ = Vertex(xMax, 0, zMax);
    minXZ = Vertex(xMin, 0, zMin);
}

const Vertex &Object::getCenter()
{
    if (!center.has_value())
        calcGeometricParams();

    return *center;
}

const Vertex &Object::getMaxXZ()
{
    if (!maxXZ.has_value())
        calcGeometricParams();

    return *maxXZ;
}

const Vertex &Object::getMinXZ()
{
    if (!minXZ.has_value())
        calcGeometricParams();

    return *minXZ;
}
