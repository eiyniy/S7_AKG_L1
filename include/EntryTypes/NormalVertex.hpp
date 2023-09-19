#pragma once

#include <BaseVertex.hpp>
#include <Types.hpp>
#include <vector>
#include <string>
#include <optional>

class NormalVertex : public BaseVertex
{
public:
    NormalVertex(std::string &line, ParseType parseType);

    double getI();
    double getJ();
    double getK();
};
