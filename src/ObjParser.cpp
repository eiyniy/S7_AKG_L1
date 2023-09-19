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

optional<string> ObjParser::getNextPart(string::iterator iter, string::iterator iterEnd, char divider)
{
    if (iter >= iterEnd)
        return {};

    auto iterSecond = iter;

    while (*iterSecond != divider && iterSecond != iterEnd)
        ++iterSecond;

    return string(iter, iterSecond);
}

void ObjParser::moveToNext(string::iterator *iter, std::string::iterator iterEnd, char divider)
{
    while (*iter != iterEnd && **iter != divider)
        (*iter)++;

    while (*iter != iterEnd && **iter == divider)
        (*iter)++;
}

#pragma endregion Static

ObjInfo *ObjParser::parseEntries(ParseType parseType)
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
            info->addVertex(Vertex(line, parseType));
            break;

        case EntryType::TextureVertex:
            info->addTVertex(TextureVertex(line, parseType));
            break;

        case EntryType::NormalVertex:
            info->addNVertex(NormalVertex(line, parseType));
            break;

        case EntryType::Polygon:
            info->addPolygon(Polygon(line, parseType));

        default:
            break;
        }
    }

    readStream.clear();
    readStream.seekg(0, std::ios::beg);

    return info;
}
