#include <Camera.hpp>
#include <cmath>

Camera::Camera(
    const Matrix<4, 1> &p_position,
    const Matrix<4, 1> &p_target,
    Point &p_resolution,
    const int p_fov)
    : position(p_position),
      target(p_target),
      resolution(p_resolution),
      fov(p_fov * M_PI / 180)
{
}
