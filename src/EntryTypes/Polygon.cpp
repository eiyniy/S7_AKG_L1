#include <Polygon.hpp>
#include <ObjParser.hpp>
#include <Math.hpp>
#include <string.h>

using namespace std;

Indexes::Indexes(std::vector<std::optional<int>> &indexes)
{
    if (!indexes.at(0).has_value() || indexes.size() != 3)
        throw invalid_argument("Invalid argument");

    vertexId = indexes[0].value();
    tVertexId = indexes[1];
    nVertexId = indexes[2];
}

Polygon::Polygon(std::string &line)
{
    auto entryType = ObjParser::getEntryType(line);
    if (entryType != EntryType::Polygon)
        throw std::invalid_argument("Could not parse value");

    std::optional<std::string> strPart;
    auto accumulator = vector<optional<int>>(3, nullopt);

    auto iter = line.begin();
    auto iterEnd = line.end();

    ObjParser::moveToNext(&iter, iterEnd, ' ');

    while (strPart = ObjParser::getNextPart(iter, iterEnd, ' '))
    {
        auto iterInner = strPart.value().begin();

        std::optional<std::string> strPartInner;

        int i = 0;
        while (strPartInner = ObjParser::getNextPart(iterInner, strPart.value().end(), '/', true))
        {
            accumulator[i] = Math::optStoi(strPartInner.value());
            ObjParser::moveToNext(&iterInner, strPart.value().end(), '/', true);
            i++;
        }

        values.push_back(Indexes(accumulator));

        ObjParser::moveToNext(&iter, iterEnd, ' ');
    }
}
