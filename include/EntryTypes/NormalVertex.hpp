#pragma once

#include <BaseVertex.hpp>
#include <vector>
#include <string>
#include <optional>

class NormalVertex : public BaseVertex
{
public:
    NormalVertex(std::string &line);

    double getI();
    double getJ();
    double getK();
};
