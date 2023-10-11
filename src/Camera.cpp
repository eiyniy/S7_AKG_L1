#include <Camera.hpp>
#include <cmath>

Camera::Camera(
    const Matrix<4, 1> &p_position,
    const Matrix<4, 1> &p_target,
    Dot &p_resolution,
    const int p_fov)
    : position(p_position),
      target(p_target),
      resolution(p_resolution),
      fov(p_fov * M_PI / 180)
{
}

Matrix<4, 1> &Camera::getPosition()
{
    return position;
}

Matrix<4, 1> &Camera::getTarget()
{
    return target;
}

const Matrix<4, 1> &Camera::cGetPosition() const
{
    return position;
}

const Matrix<4, 1> &Camera::cGetTarget() const
{
    return target;
}

const Dot &Camera::cGetResolution() const
{
    return resolution;
}

Dot &Camera::getResolution()
{
    return resolution;
}

const int Camera::getFOV() const
{
    return fov;
}

const double Camera::getAspect() const
{
    return ((double)resolution.x) / resolution.y;
}
