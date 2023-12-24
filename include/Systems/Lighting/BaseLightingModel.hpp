#pragma once

#include <SFML/Graphics/Color.hpp>
#include <vector>
#include <Matrix.hpp>

class BaseLightingModel
{
public:
    [[nodiscard]] virtual sf::Color getLightIntensity(
        const Matrix<4, 1> &normal,
        const sf::Color &diffuseColor,
        const Matrix<4, 1> &lightDirection,
        const Matrix<4, 1> &sightDirection,
        const Matrix<4, 1> &mrao) const = 0;
};
