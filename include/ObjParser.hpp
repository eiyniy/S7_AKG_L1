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

    ObjInfo *parseEntries();

    static std::optional<EntryType> getEntryType(std::string &line);
    static std::optional<std::string> getNextPart(std::string::iterator iter, std::string::iterator iterEnd, char divider);
    static void moveToNext(std::string::iterator *iter, std::string::iterator iterEnd, char divider);

private:
    std::ifstream readStream;
    std::string pathToFile;
};
