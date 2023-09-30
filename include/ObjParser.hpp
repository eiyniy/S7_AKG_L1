#pragma once

#include <ObjInfo.hpp>
#include <Types.hpp>
#include <string>
#include <optional>
#include <fstream>
#include <vector>

class ObjParser
{
public:
    ObjParser(std::string p_pathToFile);

    std::string *readFile();
    ObjInfo *parseEntries(const std::string &fileContent);
    void parseEntry(const std::string &line, ObjInfo *result) const;

    static std::optional<EntryType> getEntryType(const std::string &line);
    static std::optional<std::string> getNextPart(
        std::string::const_iterator *iter,
        std::string::const_iterator iterEnd,
        const char divider,
        const bool allowEmpty = false);

private:
    std::ifstream readStream;
    std::string pathToFile;
};
