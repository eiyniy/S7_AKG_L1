#include <BaseVertex.hpp>
#include <ObjParser.hpp>
#include <Timer.hpp>
#include <array>
#include <stdexcept>

BaseVertex::BaseVertex(std::string &line, EntryType type)
{
    auto entryType = ObjParser::getEntryType(line);
    if (entryType != EntryType::Vertex &&
        entryType != EntryType::TextureVertex &&
        entryType != EntryType::NormalVertex)
        throw std::logic_error("Could not parse value");

    std::optional<std::string> strPart;
    static auto accumulator = std::array<std::optional<double>, 4>();

    auto iter = line.begin();
    auto iterEnd = line.cend();

    ObjParser::getNextPart(&iter, iterEnd, ' ');

    int i = 0;
    while ((strPart = ObjParser::getNextPart(&iter, line.end(), ' ')))
    {
        accumulator[i] = stod(strPart.value());
        ++i;
    }

    if (i < 1)
        throw std::logic_error("Can't parse value");

    vector = CoordinateVector(
        accumulator[0].value(),
        accumulator[1].value_or(0),
        accumulator[2].value_or(0));
    v4 = accumulator[3].value_or(1);

    accumulator.fill(std::nullopt);
}
