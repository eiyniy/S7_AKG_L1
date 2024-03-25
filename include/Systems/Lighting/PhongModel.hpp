#pragma once

#include <BaseLightingModel.hpp>

class PhongModel : public BaseLightingModel
{
public:
    PhongModel(
        double _ambientCoeff,
        double _diffuseCoeff,
        double _specularCoeff);

    [[nodiscard]] sf::Color getLightIntensity(
        const Vector<4> &normal,
        const Vector<4> &diffuseColorMatrix,
        const Vector<4> &lightDirection,
        const Vector<4> &sightDirection,
        const Vector<4> &mrao) const override;

    double cGetAmbientCoeff() const;

private:
    const double ambientCoeff;
    const double diffuseCoeff;
    const double reflectionCoeff;
};
