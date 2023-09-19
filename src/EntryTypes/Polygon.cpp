#include <Polygon.hpp>
#include <ObjParser.hpp>
#include <string.h>
#include <iostream>
#include <sstream>

using namespace std;

Polygon::Polygon(std::string &line, ParseType parseType)
{
    auto entryType = ObjParser::getEntryType(line);
    if (entryType != EntryType::Polygon)
        throw std::invalid_argument("Could not parse value");

    std::optional<std::string> strPart;

    switch (parseType)
    {
    case ParseType::Iterator:
    {
        auto iter = line.begin();
        auto iterEnd = line.end();

        ObjParser::moveToNext(&iter, iterEnd, ' ');

        while (strPart = ObjParser::getNextPart(iter, iterEnd, ' '))
        {
            auto iterInner = strPart.value().begin();

            std::optional<std::string> strPartInner;
            while (strPartInner = ObjParser::getNextPart(iterInner, strPart.value().end(), '/'))
            {
                this->append(stoi(strPartInner.value()));
                ObjParser::moveToNext(&iterInner, strPart.value().end(), '/');
            }

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
        {
            char *cstrInnerPt = new char[strlen(cstrPart) + 1];
            strcpy(cstrInnerPt, cstrPart);

            char *cstrPartInner = strtok(cstrInnerPt, "/");
            do
            {
                this->append(stod(cstrPartInner));
            } while (cstrPartInner = strtok(NULL, "/"));

            delete[] cstrInnerPt;
            break;
        }

        delete[] cstrPt;
        break;

        break;
    }
    case ParseType::Stringstream:
    {
        istringstream ss(line);
        string strPart;

        getline(ss, strPart, ' ');
        while (getline(ss, strPart, ' '))
        {
            istringstream ssInner(strPart);
            string strPartInner;

            while (getline(ssInner, strPartInner, '/'))
                this->append(stod(strPartInner));
        }

        break;
    }
    default:
        break;
    }
}
