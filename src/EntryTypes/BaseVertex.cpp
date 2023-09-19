#include <BaseVertex.hpp>
#include <ObjParser.hpp>
#include <Types.hpp>
#include <string.h>
#include <iostream>
#include <sstream>

using namespace std;

BaseVertex::BaseVertex(std::string &line, ParseType parseType)
{
    auto entryType = ObjParser::getEntryType(line);
    if (entryType != EntryType::Vertex &&
        entryType != EntryType::TextureVertex &&
        entryType != EntryType::NormalVertex)
        throw std::invalid_argument("Could not parse value");

    std::optional<std::string> strPart;

    switch (parseType)
    {
    case ParseType::Iterator:
    {
        auto iter = line.begin();
        auto iterEnd = line.end();

        ObjParser::moveToNext(&iter, iterEnd, ' ');

        while (strPart = ObjParser::getNextPart(iter, line.end(), ' '))
        {
            this->append(stod(strPart.value()));
            ObjParser::moveToNext(&iter, iterEnd, ' ');
        }

        break;
    }
    case ParseType::Strtok:
    {
        char *cstrPt = new char[line.length() + 1];
        strcpy(cstrPt, line.c_str());

        char *cstrPart = strtok(cstrPt, " ");

        while (cstrPart = strtok(NULL, " "))
            this->append(stod(cstrPart));

        delete[] cstrPt;
        break;
    }
    case ParseType::Stringstream:
    {
        istringstream ss(line);
        string strPart;

        while (getline(ss, strPart, ' '))
        {
            if (isdigit(*strPart.begin()) || *strPart.begin() == '-')
                this->append(stod(strPart));
        }

        break;
    }
    default:
        break;
    }
}