#include <VertexIndexes.hpp>
#include <ObjParser.hpp>
#include <Math.hpp>
#include <Timer.hpp>
#include <vector>
#include <chrono>
#include <array>

using namespace std;

VertexIndexes::VertexIndexes(std::string &str)
{
    optional<string> strPart;
    static auto accumulator = array<optional<double>, 3>();

    auto iter = str.begin();
    auto iterEnd = str.cend();

    int i = 0;
    while ((strPart = ObjParser::getNextPart(&iter, iterEnd, '/', true)))
    {
        accumulator[i] = Math::optStoi(strPart.value());
        ++i;
    }

    if (!accumulator[0].has_value())
        throw invalid_argument("Invalid argument");

    vertexId = accumulator[0].value();
    tVertexId = accumulator[1];
    nVertexId = accumulator[2];

    accumulator.fill(nullopt);
}

VertexIndexes::VertexIndexes() {}
