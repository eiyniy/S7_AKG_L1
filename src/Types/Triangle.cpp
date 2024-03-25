#include <Triangle.hpp>
#include <ObjParser.hpp>
#include <Math.hpp>
#include <EarClipper.hpp>

Triangle::Triangle(
    const std::vector<VertexIds> &indexes,
    const std::optional<std::string> &_materialName)
    : vertexIndexesCount((int)indexes.size()),
      materialName(_materialName)
{
    if (values.size() != 3)
        throw std::logic_error("Invalid argument");

    values[0] = indexes[0];
    values[1] = indexes[1];
    values[2] = indexes[2];

    color = sf::Color(rand() % 255, rand() % 255, rand() % 255);
}

int Triangle::cGetVertexIdsCount() const
{
    return vertexIndexesCount;
}

const VertexIds &Triangle::cGetVertexIds(const int i) const
{
    if (i > 2)
        throw std::invalid_argument("Could not get VertexIds");

    return values[i];
}

std::vector<Triangle> Triangle::parseAndTriangulate(
    const std::string &line,
    const std::vector<Vector<4>> &vertices,
    const std::optional<std::string> &materialName)
{
    const auto accumulator = parseInner(line);
    return EarClipper::triangulate(accumulator, vertices, materialName);
}

const Vector<4> &Triangle::getFlatNormal(const std::vector<Vector<4>> &vertices)
{
    if (!normal.has_value())
    {
        const auto &a = vertices.at(cGetVertexIds(0).cGetVertexId() - 1);
        const auto &b = vertices.at(cGetVertexIds(1).cGetVertexId() - 1);
        const auto &c = vertices.at(cGetVertexIds(2).cGetVertexId() - 1);

        const auto v0 = b - a;
        const auto v1 = c - a;

        normal = v0.vectorMultiply(v1);
        normal->normalize();
    }

    return *normal;
}

const Vector<4> Triangle::getPhongNormal(
    const std::vector<Vector<4>> &nVertices,
    const double b0, const double b1, const double b2)
{
    Vector<4> phongNormal;

    const auto nId0 = cGetVertexIds(0).cGetNormalVertexId();
    const auto nId1 = cGetVertexIds(1).cGetNormalVertexId();
    const auto nId2 = cGetVertexIds(2).cGetNormalVertexId();

    if (!nId0.has_value() || !nId1.has_value() || !nId2.has_value())
        throw std::runtime_error("Can not get normal");

    const auto &aNormal = nVertices.at(*nId0 - 1);
    const auto &bNormal = nVertices.at(*nId1 - 1);
    const auto &cNormal = nVertices.at(*nId2 - 1);

    phongNormal = aNormal * b0 + bNormal * b1 + cNormal * b2;
    phongNormal.normalize();

    return phongNormal;
}

const Vector<4> &Triangle::getCenter(const std::vector<Vector<4>> &vertices)
{
    // if (!center.has_value())
    // {
    const auto &a = vertices.at(cGetVertexIds(0).cGetVertexId() - 1);
    const auto &b = vertices.at(cGetVertexIds(1).cGetVertexId() - 1);
    const auto &c = vertices.at(cGetVertexIds(2).cGetVertexId() - 1);

    center = {
        (a.cGetX() + b.cGetX() + c.cGetX()) / 3,
        (a.cGetY() + b.cGetY() + c.cGetY()) / 3,
        (a.cGetZ() + b.cGetZ() + c.cGetZ()) / 3,
    };
    // }

    return *center;
}

std::vector<VertexIds> Triangle::parseInner(const std::string &line)
{
    auto entryType = ObjParser::getEntryType(line);
    if (entryType != ObjEntryType::Polygon)
        throw std::logic_error("Could not parse polygon");

    std::vector<VertexIds> accumulator{};
    accumulator.reserve(3);

    auto iter = line.cbegin();
    auto iterEnd = line.cend();

    ObjParser::getNextPart(&iter, iterEnd, ' ');

    int i = 0;
    while (auto strPart = ObjParser::getNextPart(&iter, iterEnd, ' '))
    {
        accumulator.emplace_back(VertexIds::parse(*strPart));
        ++i;
    }

    return accumulator;
}
