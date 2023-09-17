#pragma once

#include <Entry.hpp>
#include <string>

class Polygon : public Entry<double>
{
public:
    Polygon(std::string &line);
};
