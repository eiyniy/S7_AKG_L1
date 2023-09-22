#pragma once

#include <Types.hpp>
#include <CoordinateVector.hpp>
#include <string>
#include <optional>

class BaseVertex
{
public:
    BaseVertex(std::string &line, EntryType type);

protected:
    CoordinateVector vector;
    double v4;
};
