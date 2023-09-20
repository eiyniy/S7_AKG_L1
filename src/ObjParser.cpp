#include <ObjParser.hpp>
#include <Types.hpp>
#include <optional>
#include <filesystem>
#include <functional>
#include <iostream>

using namespace std;

ObjParser::ObjParser(string p_pathToFile)
{
    if (!std::filesystem::exists(p_pathToFile))
        throw std::runtime_error("Could not open file");

    pathToFile = p_pathToFile;

    readStream.open(pathToFile);
    if (!readStream.is_open())
        throw std::runtime_error("Could not open file");
}

#pragma region Static

optional<EntryType> ObjParser::getEntryType(std::string &line)
{
    auto iter = line.begin();
    auto iterEnd = line.end();

    auto type = getNextPart(iter, iterEnd, ' ');

    if (type && type == "v")
        return EntryType::Vertex;
    else if (type && type == "vt")
        return EntryType::TextureVertex;
    else if (type && type == "vn")
        return EntryType::NormalVertex;
    else if (type && type == "f")
        return EntryType::Polygon;
    else
        return {};
}

optional<string> ObjParser::getNextPart(string::iterator iter, string::iterator iterEnd, char divider, bool allowEmpty)
{
    if (iter >= iterEnd)
        return std::nullopt;

    auto iterSecond = iter;

    while (*iterSecond != divider && iterSecond != iterEnd)
        ++iterSecond;

    /*if (allowEmpty && *iter == divider && *iterSecond == divider)
        return "";
    else*/
    return string(iter, iterSecond);
}

void ObjParser::moveToNext(string::iterator *iter, std::string::iterator iterEnd, char divider, bool allowEmpty)
{
    while (*iter != iterEnd && **iter != divider)
        (*iter)++;

    if (allowEmpty)
        (*iter)++;
    else
    {
        while (*iter != iterEnd && **iter == divider)
            (*iter)++;
    }
}

#pragma endregion Static

ObjInfo *ObjParser::parseEntries()
{
    auto info = new ObjInfo();

    string line;
    optional<EntryType> type;

    while (getline(readStream, line))
    {
        if (!(type = getEntryType(line)))
            continue;

        switch (type.value())
        {
        case EntryType::Vertex:
            info->addVertex(Vertex(line));
            break;

        case EntryType::TextureVertex:
            info->addTVertex(TextureVertex(line));
            break;

        case EntryType::NormalVertex:
            info->addNVertex(NormalVertex(line));
            break;

        case EntryType::Polygon:
            info->addPolygon(Polygon(line));

        default:
            break;
        }
    }

    readStream.clear();
    readStream.seekg(0, std::ios::beg);

    return info;
}
