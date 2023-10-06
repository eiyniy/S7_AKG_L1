#include <ObjParser.hpp>
#include <Types.hpp>
#include <Timer.hpp>
#include <optional>
#include <filesystem>
#include <functional>
#include <iostream>
#include <stdexcept>
#include <sstream>

ObjParser::ObjParser(std::string p_pathToFile)
{
    if (!std::filesystem::exists(p_pathToFile))
        throw std::logic_error("Could not open file");

    std::swap(pathToFile, p_pathToFile);

    readStream.open(pathToFile);
    if (!readStream.is_open())
        throw std::logic_error("Could not open file");
}

#pragma region Static

std::optional<EntryType> ObjParser::getEntryType(const std::string &line)
{
    auto iter = line.begin();

    auto type = getNextPart(&iter, line.end(), ' ');

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

std::optional<std::string> ObjParser::getNextPart(
    std::string::const_iterator *iter,
    std::string::const_iterator iterEnd,
    char divider,
    bool allowEmpty)
{
    if (*iter >= iterEnd)
        return std::nullopt;

    auto iterSecond = *iter;

    while (iterSecond < iterEnd && *iterSecond != divider)
        ++iterSecond;

    auto result = std::string(*iter, iterSecond);

    *iter = iterSecond;

    if (allowEmpty)
    {
        do
        {
            ++(*iter);
        } while (*iter < iterEnd && **iter != divider && **iter != '-' && !isdigit(**iter));
    }
    else
    {
        while (*iter < iterEnd && (**iter == divider || **iter == '\r'))
            ++(*iter);
    }

    return result;
}

#pragma endregion Static

ObjInfo *ObjParser::parseEntries(const std::string &fileContent)
{
    auto info = new ObjInfo();

    auto iter = fileContent.cbegin();
    auto iterEnd = fileContent.cend();

    std::istringstream ss(fileContent);
    std::string line;

    while (getline(ss, line, '\n'))
        parseEntry(line, info);

    readStream.clear();
    readStream.seekg(0, std::ios::beg);

    return info;
}

void ObjParser::parseEntry(const std::string &line, ObjInfo *result) const
{
    std::optional<EntryType> type;
    if (!(type = getEntryType(line)))
        return;

    switch (*type)
    {
    case EntryType::Vertex:
        result->addVertex(Vertex::parse(line));
        break;
    case EntryType::TextureVertex:
        result->addTVertex(TextureVertex::parse(line));
        break;
    case EntryType::NormalVertex:
        result->addNVertex(NormalVertex::parse(line));
        break;
    case EntryType::Polygon:
        result->addPolygon(Polygon(line));
        break;
    }
}

std::string *ObjParser::readFile()
{
    readStream.seekg(0, std::ios::end);
    auto size = readStream.tellg();
    auto buffer = new std::string(size, ' ');
    readStream.seekg(0);
    readStream.read(&((*buffer)[0]), size);

    return buffer;
}
