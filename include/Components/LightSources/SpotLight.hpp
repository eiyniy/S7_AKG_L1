#pragma once

#include <BaseLightSource.hpp>

class SpotLight : public BaseLightSource
{
public:
    explicit SpotLight(Matrix<4, 1> &_position);

    Matrix<4, 1> getLightDirection(const Matrix<4, 1> &target) const override;

private:
    Matrix<4, 1> position;
};
