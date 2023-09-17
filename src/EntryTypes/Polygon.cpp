#include <Polygon.hpp>
#include <ObjParser.hpp>

using namespace std;

Polygon::Polygon(std::string &line)
{
    auto entryType = ObjParser::getEntryType(line);
    if (entryType != EntryType::Polygon)
        throw std::invalid_argument("Could not parse value");

    auto iter = line.begin();
    auto iterEnd = line.end();

    ObjParser::moveToNext(&iter, iterEnd, ' ');

    std::optional<std::string> strPart;
    while (strPart = ObjParser::getNextPart(iter, iterEnd, ' '))
    {
        auto iterInner = strPart.value().begin();

        std::optional<std::string> strPartInner;
        while (strPartInner = ObjParser::getNextPart(iterInner, strPart.value().end(), '/'))
        {
            this->append(stoi(strPartInner.value()));
            ObjParser::moveToNext(&iterInner, strPart.value().end(), '/');
        }

        ObjParser::moveToNext(&iter, iterEnd, ' ');
    }
}
