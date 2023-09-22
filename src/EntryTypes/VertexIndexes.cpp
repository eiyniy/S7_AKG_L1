#include <VertexIndexes.hpp>
#include <ObjParser.hpp>
#include <Math.hpp>
#include <Timer.hpp>
#include <vector>
#include <chrono>
#include <array>

VertexIndexes::VertexIndexes(std::string &str)
{
    std::optional<std::string> strPart;
    static auto accumulator = std::array<std::optional<double>, 3>();

    auto iter = str.begin();
    auto iterEnd = str.cend();

    int i = 0;
    while ((strPart = ObjParser::getNextPart(&iter, iterEnd, '/', true)))
    {
        accumulator[i] = Math::optStoi(strPart.value());
        ++i;
    }

    if (!accumulator[0].has_value())
        throw std::logic_error("Invalid argument");

    vertexId = accumulator[0].value();
    tVertexId = accumulator[1];
    nVertexId = accumulator[2];

    accumulator.fill(std::nullopt);
}

VertexIndexes::VertexIndexes() {}
