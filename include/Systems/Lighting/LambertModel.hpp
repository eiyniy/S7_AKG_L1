#pragma once

#include <BaseLightingModel.hpp>

class LambertModel : public BaseLightingModel
{
public:
    [[nodiscard]] sf::Color getLightIntensity(
        const Vector<4> &normal,
        const Vector<4> &diffuseColorMatrix,
        const Vector<4> &lightDirection,
        const Vector<4> &sightDirection,
        const Vector<4> &mrao) const override;
};
