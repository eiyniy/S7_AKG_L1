#include <Polygon.hpp>
#include <ObjParser.hpp>
#include <Math.hpp>
#include <EarClipper.hpp>

SValues::SValues(const std::vector<VertexIds> &values)
    : v4(std::nullopt)
{
    if (values.size() != 3 && values.size() != 4)
        throw std::logic_error("Invalid argument");

    v1 = values[0];
    v2 = values[1];
    v3 = values[2];

    if (values.size() == 4)
        v4 = values[3];
}

Polygon::Polygon(const std::vector<VertexIds> &indexes)
{
    vertexIndexesCount = (int)indexes.size();

    if (vertexIndexesCount <= 4)
    {
        storageMode = StorageMode::Static;
        sValues = SValues(indexes);
    }
    else
    {
        storageMode = StorageMode::Dynamic;
        dValues = indexes;
    }

    color = sf::Color(rand() % 255, rand() % 255, rand() % 255);
}

Polygon Polygon::parse(const std::string &line)
{
    const auto accumulator = parseInner(line);
    return Polygon{accumulator};
}

int Polygon::cGetVertexIdsCount() const
{
    return vertexIndexesCount;
}

const VertexIds &Polygon::cGetVertexIds(const int i) const
{
    switch (storageMode)
    {
    case StorageMode::Static:
        switch (i)
        {
        case 0:
            return sValues->v1;
        case 1:
            return sValues->v2;
        case 2:
            return sValues->v3;
        case 3:
            if (!sValues->v4.has_value())
                throw std::invalid_argument("Could not get VertexIds");

            return *sValues->v4;
        default:
            throw std::invalid_argument("Could not get VertexIds");
        }
    case StorageMode::Dynamic:
        return dValues->at(i);
    }
}

void Polygon::moveValuesToDynamic()
{
    if (!sValues.has_value())
        throw std::logic_error("Could not store polygon");

    storageMode = StorageMode::Dynamic;

    dValues = std::vector<VertexIds>(4);

    dValues->at(0) = sValues->v1;
    dValues->at(1) = sValues->v2;
    dValues->at(2) = sValues->v3;
    dValues->at(3) = *sValues->v4;
}

std::vector<Polygon> Polygon::parseAndTriangulate(
    const std::string &line,
    const std::vector<Matrix<4, 1>> &vertices)
{
    const auto accumulator = parseInner(line);
    return EarClipper::triangulate(accumulator, vertices);
}

const Matrix<4, 1> &Polygon::getNormal(const std::vector<Matrix<4, 1>> &vertices)
{
    if (!normal.has_value())
    {
        const auto &a = vertices.at(cGetVertexIds(0).cGetVertexId() - 1);
        const auto &b = vertices.at(cGetVertexIds(1).cGetVertexId() - 1);
        const auto &c = vertices.at(cGetVertexIds(2).cGetVertexId() - 1);

        const auto v0 = b - a;
        const auto v1 = c - a;

        auto tNormal = v0.vectorMultiply(v1);
        tNormal.normalize();

        normal = tNormal;
    }

    return *normal;
}

const Matrix<4, 1> &Polygon::getCenter(const std::vector<Matrix<4, 1>> &vertices)
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

std::vector<VertexIds> Polygon::parseInner(const std::string &line)
{
    auto entryType = ObjParser::getEntryType(line);
    if (entryType != EntryType::Polygon)
        throw std::logic_error("Could not parse value");

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
