#include <Polygon.hpp>
#include <ObjParser.hpp>
#include <Math.hpp>
#include <string.h>

using namespace std;

Values::Values(std::vector<std::optional<VertexIndexes>> &values)
{
    if (!values.at(0).has_value() || !values.at(1).has_value() || !values.at(2).has_value())
        throw invalid_argument("Invalid argument");

    v1 = values[0].value();
    v2 = values[1].value();
    v3 = values[2].value();
    v4 = values[3];
}

Polygon::Polygon(std::string &line)
{
    auto entryType = ObjParser::getEntryType(line);
    if (entryType != EntryType::Polygon)
        throw std::invalid_argument("Could not parse value");

    std::optional<std::string> strPart;
    auto indexesAcc = vector<optional<VertexIndexes>>(4, nullopt);

    setStorageMode(line);

    if (storageMode == PolygonStorageMode::Dynamic)
    {
        dValues = std::make_optional(vector<VertexIndexes>());
        dValues.value().reserve(5);
    }

    auto iter = line.begin();
    auto iterEnd = line.end();

    ObjParser::moveToNext(&iter, iterEnd, ' ');

    int j = 0;
    while (strPart = ObjParser::getNextPart(iter, iterEnd, ' '))
    {
        if (storageMode == PolygonStorageMode::Dynamic)
            dValues.value().push_back(VertexIndexes(strPart.value()));
        else
            indexesAcc[j] = VertexIndexes(strPart.value());

        ObjParser::moveToNext(&iter, iterEnd, ' ');
        j++;
    }

    if (storageMode == PolygonStorageMode::Static)
        sValues = Values(indexesAcc);
}

// TODO replace with static default & remove if more then 4
void Polygon::setStorageMode(std::string &line)
{
    auto iter = line.begin();
    auto iterEnd = line.end();

    int count = 0;

    ObjParser::moveToNext(&iter, iterEnd, ' ');

    while (ObjParser::getNextPart(iter, iterEnd, ' '))
    {
        ++count;
        ObjParser::moveToNext(&iter, iterEnd, ' ');
    }

    if (count <= 4)
        storageMode = PolygonStorageMode::Static;
    else
        storageMode = PolygonStorageMode::Dynamic;
}
