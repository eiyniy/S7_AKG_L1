#pragma once

#include <Types.hpp>
#include <Matrix.hpp>
#include <CoordinateVector.hpp>
#include <string>
#include <optional>

class BaseVertex
{
public:
    BaseVertex(std::string &line, EntryType type);

    static BaseVertex fromMatrix(Matrix &m);

    operator CoordinateVector() const;

protected:
    double v1, v2, v3, v4;

private:
    BaseVertex();
};
