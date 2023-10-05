#pragma once

#include <BaseVertex.hpp>
#include <string>

class NormalVertex : public BaseVertex
{
public:
    NormalVertex(const double i, const double j, const double k);

    static NormalVertex parse(const std::string &line);

    const double getI() const;
    const double getJ() const;
    const double getK() const;
};
