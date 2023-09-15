#pragma once

#include <string>
#include <fstream>

class ObjParser
{
public:
    ObjParser(std::string p_pathToFile);

private:
    std::ifstream readStream;
    std::string pathToFile;
};