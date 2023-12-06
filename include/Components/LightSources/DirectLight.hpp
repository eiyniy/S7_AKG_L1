#pragma once

#include <BaseLightSource.hpp>

class DirectLight : public BaseLightSource {
public:
    DirectLight(Matrix<4, 1> direction);

    Matrix<4, 1> getLightDirection(const Matrix<4, 1> target) const override;

private:
    Matrix<4, 1> direction;
};
