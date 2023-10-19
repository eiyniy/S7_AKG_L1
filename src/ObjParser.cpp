#include <optional>
#include <filesystem>
#include <iostream>
#include <sstream>
#include <mutex>
#include <ObjParser.hpp>
#include <Enums.hpp>
#include <Timer.hpp>
#include <ThreadPool.hpp>

ObjParser::ObjParser(const std::string &_pathToFile)
{
    if (!std::filesystem::exists(_pathToFile))
        throw std::logic_error("Could not open file");

    pathToFile = _pathToFile;

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

Object *ObjParser::parseEntries(const std::string &fileContent)
{
    const auto timeStart = std::chrono::high_resolution_clock::now();

    const auto lines = splitByLines(fileContent);

    /*
    // const int threadsCount = (unsigned int)ceil(lines.size() / 10000.f);
    const int threadsCount = std::min(
        (unsigned int)ceil(lines.size() / 10000.f),
        ThreadPool::getInstance().getThreadsCount());

    const double size = lines.size() / (double)threadsCount;

    for (int i = 0; i < threadsCount; ++i)
    {
        const int begin = floor(size * i);
        const int end = floor(size * (i + 1)) - 1;

        ThreadPool::getInstance().enqueue(
            [this, i, begin, end, &lines]()
            {
                for (int j = begin; j <= end; ++j)
                    parseEntry(lines[j]);
            });
    }

    ThreadPool::getInstance().waitAll();
    */

    // /*
    for (int j = 0; j < lines.size(); ++j)
        parseEntry(lines[j]);
    // */

    const auto timeEnd = std::chrono::high_resolution_clock::now();
    auto parseTime = std::chrono::duration_cast<std::chrono::milliseconds>(timeEnd - timeStart).count();
    std::cout << "Parse time: " << parseTime << " ms" << std::endl;

    return new Object(
        vertexes,
        tVertexes,
        nVertexes,
        polygons,
        sf::Color::White);
}

void ObjParser::parseEntry(const std::string &line)
{
    const auto type = getEntryType(line);
    if (!type.has_value())
        return;

    switch (*type)
    {
    case EntryType::Vertex:
        vertexes.emplace_back(Vertex::parse(line));
        break;
    case EntryType::TextureVertex:
        tVertexes.emplace_back(TextureVertex::parse(line));
        break;
    case EntryType::NormalVertex:
        nVertexes.emplace_back(NormalVertex::parse(line));
        break;
    case EntryType::Polygon:
        polygons.emplace_back(Polygon::parse(line));
        break;
    }
}

std::vector<std::string> ObjParser::splitByLines(const std::string &string)
{
    auto result = std::vector<std::string>{};
    auto ss = std::stringstream{string};

    for (std::string line; std::getline(ss, line, '\n');)
        result.emplace_back(line);

    return result;
}

std::string ObjParser::readFile()
{
    readStream.seekg(0, std::ios::end);
    auto size = readStream.tellg();
    auto buffer = std::string(size, ' ');
    readStream.seekg(0);
    readStream.read(&(buffer[0]), size);
    readStream.close();

    return buffer;
}
