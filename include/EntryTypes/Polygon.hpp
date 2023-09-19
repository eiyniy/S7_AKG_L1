#pragma once

#include <Entry.hpp>
#include <Types.hpp>
#include <string>

class Polygon : public Entry<double>
{
public:
    Polygon(std::string &line, ParseType parseType);
};
