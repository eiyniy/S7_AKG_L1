#pragma once

#include <BaseLightingModel.hpp>

class LambertModel : public BaseLightingModel
{
public:
    [[nodiscard]] sf::Color getLightIntensity(
        const Matrix<4, 1> &normal,
        const sf::Color &diffuseColor,
        const Matrix<4, 1> &lightDirection,
        const Matrix<4, 1> &sightDirection,
        const Matrix<4, 1> &mrao) const override;
};
