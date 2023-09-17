#include <BaseVertex.hpp>
#include <ObjParser.hpp>

using namespace std;

BaseVertex::BaseVertex(std::string &line)
{
    auto entryType = ObjParser::getEntryType(line);
    if (entryType != EntryType::Vertex &&
        entryType != EntryType::TextureVertex &&
        entryType != EntryType::NormalVertex)
        throw std::invalid_argument("Could not parse value");

    auto iter = line.begin();
    ObjParser::moveToNext(&iter);

    std::optional<std::string> strPart;
    while (strPart = ObjParser::getNextPart(iter, line.end()))
    {
        this->append(stod(strPart.value()));
        ObjParser::moveToNext(&iter);
    }
}