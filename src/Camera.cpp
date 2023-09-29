#include <Camera.hpp>

Camera::Camera(const CoordinateVector &p_position, const CoordinateVector &p_target)
    : position(p_position), target(p_target) {}

CoordinateVector &Camera::getPosition()
{
    return position;
}

CoordinateVector &Camera::getTarget()
{
    return target;
}
