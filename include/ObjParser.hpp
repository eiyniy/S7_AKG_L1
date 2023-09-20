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
    ObjInfo *parseEntries(std::string &fileContent);
    void parseEntry(std::string &line, ObjInfo *result);

    static std::optional<EntryType> getEntryType(std::string &line);
    static std::optional<std::string> getNextPart(
        std::string::iterator *iter,
        std::string::const_iterator iterEnd,
        char divider,
        bool allowEmpty = false);

private:
    std::ifstream readStream;
    std::string pathToFile;

    const char *allowedChars;
};
