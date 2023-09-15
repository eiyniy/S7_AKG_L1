#include <ObjParser.hpp>
#include <filesystem>
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

    string line;
    int count = 0;

    while (getline(readStream, line))
    {
        ++count;
    }

    cout << "Lines count: " << count << endl;

    readStream.close();
}
