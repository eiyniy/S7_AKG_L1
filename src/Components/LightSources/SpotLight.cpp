#include <SpotLight.hpp>

SpotLight::SpotLight(Matrix<4, 1> &_position)
    : position(_position) {}

Matrix<4, 1> SpotLight::getLightDirection(const Matrix<4, 1> &target) const
{
    auto tDir = target - position;
    tDir.normalize();
    return tDir;
}
