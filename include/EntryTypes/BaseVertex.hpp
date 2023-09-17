#pragma once

#include <Entry.hpp>
#include <string>

class BaseVertex : public Entry<double>
{
public:
    BaseVertex(std::string &line);
};
