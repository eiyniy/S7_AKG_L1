#pragma once

#include <SFML/Graphics/Color.hpp>
#include <vector>
#include <Matrix.hpp>
#include <NormalVertex.hpp>

class BaseLightingModel {
public:
    virtual double getLightIntensity(
            const NormalVertex &normalVertex,
            const Matrix<4, 1> &lightDirection) const = 0;
};
