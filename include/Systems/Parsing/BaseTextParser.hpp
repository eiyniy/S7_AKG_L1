#pragma once

#include <string>
#include <fstream>
#include <vector>
#include <optional>

class BaseTextParser
{
public:
    BaseTextParser(const std::string &_pathToFile);

    static std::vector<std::string> splitByLines(const std::string &string);

    static std::optional<std::string> getNextPart(
        std::string::const_iterator *iter,
        std::string::const_iterator iterEnd,
        char divider,
        bool allowEmpty = false);

protected:
    std::string pathToFile;
    std::ifstream readStream;

    std::string readFile(const std::string &pathToFile);
};
