#include <VertexIndexes.hpp>
#include <ObjParser.hpp>
#include <Math.hpp>
#include <Timer.hpp>
#include <vector>
#include <chrono>

using namespace std;

VertexIndexes::VertexIndexes(std::string &str)
{
    optional<string> strPart;
    //auto accumulator = vector<optional<double>>(3, nullopt);

    auto iter = str.begin();
    auto iterEnd = str.cend();

    int i = 0;
    while (strPart = ObjParser::getNextPart(&iter, iterEnd, '/', true))
    {
        accumulator[i] = Math::optStoi(strPart.value());
        ++i;
    }

    if (!accumulator.at(0).has_value())
        throw invalid_argument("Invalid argument");

    vertexId = accumulator[0].value();
    tVertexId = accumulator[1];
    nVertexId = accumulator[2];
}

VertexIndexes::VertexIndexes() {}
