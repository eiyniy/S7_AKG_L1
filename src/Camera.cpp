#include <Camera.hpp>

Camera::Camera(const CoordinateVector &p_position, const CoordinateVector &p_target)
    : position(p_position), target(p_target) {}

const CoordinateVector &Camera::getPosition() const
{
    return position;
}

const CoordinateVector &Camera::getTarget() const
{
    return target;
}
