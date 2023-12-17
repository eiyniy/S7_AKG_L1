#include <DirectLight.hpp>

Matrix<4, 1> DirectLight::getLightDirection(const Matrix<4, 1> &target) const {
    return direction;
}

DirectLight::DirectLight(Matrix<4, 1> &direction)
        : direction(direction) {}
