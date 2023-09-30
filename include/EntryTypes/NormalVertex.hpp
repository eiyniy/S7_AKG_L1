#pragma once

#include <BaseVertex.hpp>
#include <vector>
#include <string>
#include <optional>

class NormalVertex : public BaseVertex
{
public:
    NormalVertex(const std::string &line);

    const double getI() const;
    const double getJ() const;
    const double getK() const;
};
