#include <ObjParser.hpp>
#include <Types.hpp>
#include <optional>
#include <filesystem>
#include <functional>
#include <iostream>

using namespace std;

ObjParser::ObjParser(string p_pathToFile) : allowedChars("-0123456789vtnf")
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
        return std::nullopt;

    auto iterSecond = iter;

    while (iterSecond < iterEnd && *iterSecond != divider)
        ++iterSecond;

    return string(iter, iterSecond);
}

void ObjParser::moveToNext(string::iterator *iter, std::string::iterator iterEnd, char divider, bool allowEmpty)
{
    while (*iter < iterEnd && **iter != divider)
        (*iter)++;

    if (allowEmpty)
    {
        do
        {
            (*iter)++;
        } while (*iter < iterEnd && **iter != divider && **iter != '-' && !isdigit(**iter));
    }
    else
    {
        while (*iter < iterEnd && (**iter == divider || **iter == '\r'))
            (*iter)++;
    }
}

#pragma endregion Static

ObjInfo *ObjParser::parseEntries(std::string &fileContent)
{
    auto info = new ObjInfo();

    auto iter = fileContent.begin();
    auto iterEnd = fileContent.end();

    optional<string> line;
    while (line = getNextPart(iter, iterEnd, '\n'))
    {
        parseEntry(line.value(), info);
        moveToNext(&iter, iterEnd, '\n');
    }

    readStream.clear();
    readStream.seekg(0, std::ios::beg);

    return info;
}

void ObjParser::parseEntry(std::string &line, ObjInfo *result)
{
    optional<EntryType> type;
    if (!(type = getEntryType(line)))
        return;

    switch (type.value())
    {
    case EntryType::Vertex:
        result->addVertex(Vertex(line));
        break;
    case EntryType::TextureVertex:
        result->addTVertex(TextureVertex(line));
        break;
    case EntryType::NormalVertex:
        result->addNVertex(NormalVertex(line));
        break;
    case EntryType::Polygon:
        result->addPolygon(Polygon(line));
        break;
    default:
        break;
    }
}

std::string *ObjParser::readFile()
{
    readStream.seekg(0, ios::end);
    auto size = readStream.tellg();
    auto buffer = new string(size, ' ');
    readStream.seekg(0);
    readStream.read(&((*buffer)[0]), size);

    return buffer;
}
