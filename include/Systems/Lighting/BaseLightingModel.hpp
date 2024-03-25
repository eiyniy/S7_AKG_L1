#pragma once

#include <SFML/Graphics/Color.hpp>
#include <vector>
#include <Matrix.hpp>

class BaseLightingModel
{
public:
    [[nodiscard]] virtual sf::Color getLightIntensity(
        const Vector<4> &normal,
        const Vector<4> &diffuseColorMatrix,
        const Vector<4> &lightDirection,
        const Vector<4> &sightDirection,
        const Vector<4> &mrao) const = 0;
};
