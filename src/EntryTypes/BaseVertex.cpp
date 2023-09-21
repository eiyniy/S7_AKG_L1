#include <BaseVertex.hpp>
#include <ObjParser.hpp>
#include <optional>
#include <array>

using namespace std;

BaseVertex::BaseVertex(std::string &line)
{
    auto entryType = ObjParser::getEntryType(line);
    if (entryType != EntryType::Vertex &&
        entryType != EntryType::TextureVertex &&
        entryType != EntryType::NormalVertex)
        throw std::invalid_argument("Could not parse value");

    optional<string> strPart;
    static auto accumulator = array<optional<double>, 4>();

    auto iter = line.begin();
    auto iterEnd = line.cend();

    ObjParser::getNextPart(&iter, iterEnd, ' ');

    int i = 0;
    while ((strPart = ObjParser::getNextPart(&iter, line.end(), ' ')))
    {
        accumulator[i] = stod(strPart.value());
        ++i;
    }

    if (!accumulator[0].has_value())
        throw invalid_argument("Invalid argument");

    v1 = accumulator[0].value();
    v2 = accumulator[1];
    v3 = accumulator[2];
    v4 = accumulator[3];

    accumulator.fill(nullopt);
}
