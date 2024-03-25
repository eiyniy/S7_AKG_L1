#include <DirectLight.hpp>

Vector<4> DirectLight::getLightDirection(const Vector<4> &target) const {
    return direction;
}

DirectLight::DirectLight(Vector<4> &direction)
        : direction(direction) {}
