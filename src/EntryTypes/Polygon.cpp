#include <Polygon.hpp>
#include <ObjParser.hpp>
#include <Math.hpp>
#include <Timer.hpp>
#include <array>

Values::Values(std::array<std::optional<VertexIndexes>, 4> &values)
{
    if (!values[0].has_value() || !values[1].has_value() || !values[2].has_value())
        throw std::logic_error("Invalid argument");

    v1 = values[0].value();
    v2 = values[1].value();
    v3 = values[2].value();
    v4 = values[3];
}

Values::Values() {}

Polygon::Polygon(std::string &line)
{
    auto entryType = ObjParser::getEntryType(line);
    if (entryType != EntryType::Polygon)
        throw std::logic_error("Could not parse value");

    storageMode = StorageMode::Static;

    std::optional<std::string> strPart;
    static auto accumulator = std::array<std::optional<VertexIndexes>, 4>();

    auto iter = line.begin();
    auto iterEnd = line.cend();

    ObjParser::getNextPart(&iter, iterEnd, ' ');

    int i = 0;
    while ((strPart = ObjParser::getNextPart(&iter, iterEnd, ' ')))
    {
        if (i >= 4)
            moveValuesToDynamic();

        if (storageMode == StorageMode::Dynamic)
            dValues.value().push_back(VertexIndexes(strPart.value()));
        else
            accumulator[i] = VertexIndexes(strPart.value());

        ++i;
    }

    if (storageMode == StorageMode::Static)
        sValues = Values(accumulator);

    accumulator.fill(std::nullopt);
}

void Polygon::moveValuesToDynamic()
{
    if (!sValues.has_value())
        throw std::logic_error("Could not store polygon");

    storageMode = StorageMode::Dynamic;

    dValues = std::vector<VertexIndexes>(4);

    dValues.value()[0] = sValues.value().v1;
    dValues.value()[1] = sValues.value().v2;
    dValues.value()[2] = sValues.value().v3;
    dValues.value()[3] = sValues.value().v4.value();
}
