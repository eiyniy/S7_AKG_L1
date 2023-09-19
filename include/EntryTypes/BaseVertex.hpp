#pragma once

#include <Entry.hpp>
#include <Types.hpp>
#include <string>

class BaseVertex : public Entry<double>
{
public:
    BaseVertex(std::string &line, ParseType parseType);
};
