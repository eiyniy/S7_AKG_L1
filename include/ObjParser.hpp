#pragma once

#include <Object.hpp>
#include <Enums.hpp>
#include <string>
#include <optional>
#include <fstream>
#include <vector>
#include <memory>

class ObjParser
{
public:
    ObjParser(const std::string &_pathToFile);

    std::unique_ptr<std::string> readFile();
    Object *parseEntries(const std::string &fileContent);

    static std::optional<EntryType> getEntryType(const std::string &line);
    static std::optional<std::string> getNextPart(
        std::string::const_iterator *iter,
        std::string::const_iterator iterEnd,
        const char divider,
        const bool allowEmpty = false);

private:
    std::string pathToFile;
    std::ifstream readStream;
};
