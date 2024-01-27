#include <Triangle.hpp>
#include <ObjParser.hpp>
#include <Math.hpp>
#include <EarClipper.hpp>

Triangle::Triangle(
    const std::vector<VertexIds> &indexes,
    const std::string &_materialName)
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
    const std::vector<Matrix<4, 1>> &vertices,
    const std::string &materialName)
{
    const auto accumulator = parseInner(line);
    return EarClipper::triangulate(accumulator, vertices, materialName);
}

const Matrix<4, 1> &Triangle::getNormal(const std::vector<Matrix<4, 1>> &vertices)
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

const Matrix<4, 1> &Triangle::getCenter(const std::vector<Matrix<4, 1>> &vertices)
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
