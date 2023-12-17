#pragma once

#include <SFML/Graphics/Color.hpp>
#include <vector>
#include <Matrix.hpp>
#include <NormalVertex.hpp>

class BaseLightingModel {
public:
    [[nodiscard]] virtual double getLightIntensity(
        const Matrix<4, 1> &normal,
        const Matrix<4, 1> &lightDirection) const = 0;
};
