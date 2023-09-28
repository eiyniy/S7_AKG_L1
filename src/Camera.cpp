#include <Camera.hpp>

Camera::Camera(CoordinateVector p_position)
    : position(p_position) {}

CoordinateVector &Camera::getPosition()
{
    return position;
}
