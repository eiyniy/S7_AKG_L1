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

    auto tsStart = std::chrono::high_resolution_clock::now();
    auto tsEnd = tsStart;

    long long vParseTime = 0, vtParseTime = 0, vnParseTime = 0, pParseTime = 0;

    string line;
    optional<EntryType> type;

    while (getline(readStream, line))
    {
        if (!(type = getEntryType(line)))
            continue;

        switch (type.value())
        {
        case EntryType::Vertex:
        {
            tsStart = std::chrono::high_resolution_clock::now();

            info->addVertex(Vertex(line));

            tsEnd = std::chrono::high_resolution_clock::now();
            vParseTime += std::chrono::duration_cast<std::chrono::nanoseconds>(tsEnd - tsStart).count();

            break;
        }
        case EntryType::TextureVertex:
        {
            tsStart = std::chrono::high_resolution_clock::now();

            info->addTVertex(TextureVertex(line));

            tsEnd = std::chrono::high_resolution_clock::now();
            vtParseTime += std::chrono::duration_cast<std::chrono::nanoseconds>(tsEnd - tsStart).count();

            break;
        }
        case EntryType::NormalVertex:
        {
            tsStart = std::chrono::high_resolution_clock::now();

            info->addNVertex(NormalVertex(line));

            tsEnd = std::chrono::high_resolution_clock::now();
            vnParseTime += std::chrono::duration_cast<std::chrono::nanoseconds>(tsEnd - tsStart).count();

            break;
        }
        case EntryType::Polygon:
        {
            tsStart = std::chrono::high_resolution_clock::now();

            info->addPolygon(Polygon(line));

            tsEnd = std::chrono::high_resolution_clock::now();
            pParseTime += std::chrono::duration_cast<std::chrono::nanoseconds>(tsEnd - tsStart).count();

            break;
        }
        default:
            break;
        }
    }

    cout << "Vertex parse time - " << vParseTime / 1000000 << "ms" << endl;
    cout << "Texture vertex parse time - " << vtParseTime / 1000000 << "ms" << endl;
    cout << "Normal vertex parse time - " << vnParseTime / 1000000 << "ms" << endl;
    cout << "Polygon parse time - " << pParseTime / 1000000 << "ms" << endl;
    cout << endl;

    readStream.clear();
    readStream.seekg(0, std::ios::beg);

    return info;
}
