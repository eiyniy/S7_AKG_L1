#include <VertexIndexes.hpp>
#include <ObjParser.hpp>
#include <Math.hpp>
#include <Timer.hpp>
#include <vector>
#include <chrono>
#include <array>

VertexIndexes::VertexIndexes() = default;

VertexIndexes::VertexIndexes(
    const int _vertexId,
    const std::optional<int> &_tVertexId,
    const std::optional<int> &_nVertexId)
    : vertexId(_vertexId),
      tVertexId(_tVertexId),
      nVertexId(_nVertexId) {}

VertexIndexes::VertexIndexes(std::string &str)
{
    std::optional<std::string> strPart;
    static auto accumulator = std::array<std::optional<double>, 3>();

    auto iter = str.cbegin();
    auto iterEnd = str.cend();

    int i = 0;
    while ((strPart = ObjParser::getNextPart(&iter, iterEnd, '/', true)))
    {
        accumulator[i] = Math::optStoi(*strPart);
        ++i;
    }

    if (!accumulator[0].has_value())
        throw std::logic_error("Invalid argument");

    vertexId = accumulator[0].value();
    tVertexId = accumulator[1];
    nVertexId = accumulator[2];

    accumulator.fill(std::nullopt);
}

const int VertexIndexes::cGetVertexId() const
{
    return vertexId;
}
