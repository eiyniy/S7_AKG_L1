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
        const Matrix<4, 1> &normal,
        const sf::Color &diffuseColor,
        const Matrix<4, 1> &lightDirection,
        const Matrix<4, 1> &sightDirection,
        const Matrix<4, 1> &mrao) const override;

    double cGetAmbientCoeff() const;

private:
    const double ambientCoeff;
    const double diffuseCoeff;
    const double reflectionCoeff;
};
