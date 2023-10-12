#include <VertexIndexes.hpp>
#include <ObjParser.hpp>
#include <Math.hpp>
#include <Timer.hpp>
#include <vector>
#include <chrono>
#include <array>

VertexIndexes::VertexIndexes() = default;

VertexIndexes::VertexIndexes(
    const int p_vertexId,
    const std::optional<int> &p_tVertexId,
    const std::optional<int> &p_nVertexId)
    : vertexId(p_vertexId),
      tVertexId(p_tVertexId),
      nVertexId(p_nVertexId) {}

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
