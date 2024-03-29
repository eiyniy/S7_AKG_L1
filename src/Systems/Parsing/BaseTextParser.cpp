#include <BaseTextParser.hpp>
#include <filesystem>

BaseTextParser::BaseTextParser(const std::string &_pathToFile)
{
    if (!std::filesystem::exists(_pathToFile))
        throw std::logic_error("Could not open file");

    pathToFile = _pathToFile;
}

std::vector<std::string> BaseTextParser::splitByLines(const std::string &string)
{
    auto result = std::vector<std::string>{};
    auto ss = std::stringstream{string};

    for (std::string line; std::getline(ss, line, '\n');)
        result.emplace_back(line);

    return result;
}

std::optional<std::string> BaseTextParser::getNextPart(
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

std::unique_ptr<std::string> BaseTextParser::readFile(const std::string &pathToFile)
{
    readStream.open(pathToFile);
    if (!readStream.is_open())
        throw std::logic_error("Could not open file");

    readStream.seekg(0, std::ios::end);
    auto size = readStream.tellg();
    auto buffer = std::make_unique<std::string>(size, ' ');
    readStream.seekg(0);
    readStream.read(&((*buffer)[0]), size);
    readStream.close();

    return buffer;
}
