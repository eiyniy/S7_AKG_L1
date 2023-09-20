#include <VertexIndexes.hpp>
#include <ObjParser.hpp>
#include <Math.hpp>
#include <vector>

using namespace std;

VertexIndexes::VertexIndexes(std::string &str)
{
    optional<string> strPart;
    auto accumulator = vector<optional<double>>();
    accumulator.reserve(3);

    auto iter = str.begin();

    while (strPart = ObjParser::getNextPart(iter, str.end(), '/', true))
    {
        accumulator.push_back(Math::optStoi(strPart.value()));
        ObjParser::moveToNext(&iter, str.end(), '/', true);
    }

    if (!accumulator.at(0).has_value())
        throw invalid_argument("Invalid argument");

    vertexId = accumulator[0].value();
    tVertexId = accumulator[1];
    nVertexId = accumulator[2];
}

VertexIndexes::VertexIndexes() {}
