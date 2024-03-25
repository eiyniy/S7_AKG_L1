#include <cmath>
#include <memory>
#include <Object.hpp>
#include <Converter.hpp>
#include <ThreadPool.hpp>
#include <Timer.hpp>
#include <Globals.hpp>

Vector<4> convertVertex(
    const Vector<4> &,
    const Matrix<4, 4> &,
    const Matrix<4, 4> &);

std::shared_ptr<const Material> Object::defaultMaterial = std::make_shared<const Material>(
    "__DEFAULT_MATERIAL",
    Converter::colorToMatrix(sf::Color::Black),
    Converter::colorToMatrix(sf::Color::White),
    Converter::colorToMatrix(sf::Color::Black),
    1,
    nullptr,
    nullptr,
    nullptr,
    nullptr);

Object::Object(
    const std::vector<Vector<4>> &_vertices,
    const std::vector<Vector<4>> &_tVertices,
    const std::vector<Vector<4>> &_nVertices,
    const std::vector<Triangle> &_polygons,
    std::unique_ptr<const std::map<std::string, std::shared_ptr<const Material>>> _materials)
    : vertices(_vertices),
      tVertices(_tVertices),
      nVertices(_nVertices),
      polygons(_polygons),
      materials(std::move(_materials))
{
    drawable = std::vector<Vector<4>>(vertices.size());
}

void Object::move(const Vector<4> &transition)
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

#pragma omp parallel for if (!_DEBUG)
    for (int j = 0; j < vertices.size(); ++j)
        drawable[j] = convertVertex(vertices[j], convertMatrix, viewportConvert);
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

    center = Vector<4>(cx / vCount, cy / vCount, cz / vCount);
    maxXZ = Vector<4>(xMax, 0, zMax);
    minXZ = Vector<4>(xMin, 0, zMin);
}

const Vector<4> &Object::getCenter()
{
    // if (!center.has_value())
    calcGeometricParams();

    return *center;
}

const Vector<4> &Object::getMaxXZ()
{
    // if (!maxXZ.has_value())
    calcGeometricParams();

    return *maxXZ;
}

const Vector<4> &Object::getMinXZ()
{
    // if (!minXZ.has_value())
    calcGeometricParams();

    return *minXZ;
}

Vector<4> convertVertex(
    const Vector<4> &vertex,
    const Matrix<4, 4> &toProjectionConvert,
    const Matrix<4, 4> &viewportConvert)
{
    bool isWNegative = false;

    auto vertexCopy = toProjectionConvert * vertex;

    if (vertexCopy.cGetW() <= 0)
        isWNegative = true;

    const auto w = vertexCopy.cGetW();
    const auto z = vertexCopy.cGetZ();
    vertexCopy /= w;

    vertexCopy = viewportConvert * vertexCopy;

    vertexCopy.getZ() = z;
    vertexCopy.getW() = w;

    return vertexCopy;
}
