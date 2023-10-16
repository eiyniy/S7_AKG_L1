#include <Camera.hpp>
#include <cmath>

Camera::Camera(
    const Matrix<4, 1> &_position,
    const Matrix<4, 1> &_target,
    Point &_resolution,
    const int _fov)
    : position(_position),
      target(_target),
      resolution(_resolution),
      fov(_fov * M_PI / 180) {}
