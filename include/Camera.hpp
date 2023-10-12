#pragma once

#include <Types.hpp>
#include <Matrix.hpp>

class Camera
{
private:
    Matrix<4, 1> position;
    Matrix<4, 1> target;
    Dot resolution;
    int fov;

public:
    Camera(const Matrix<4, 1> &p_position,
           const Matrix<4, 1> &p_target,
           Dot &p_resolution,
           const int p_fov);

    const Matrix<4, 1> &cGetPosition() const;
    const Matrix<4, 1> &cGetTarget() const;
    const Dot &cGetResolution() const;
    const int getFOV() const;
    const double getAspect() const;

    Matrix<4, 1> &getPosition();
    Matrix<4, 1> &getTarget();
    Dot &getResolution();
};

inline Matrix<4, 1> &Camera::getPosition()
{
    return position;
}

inline Matrix<4, 1> &Camera::getTarget()
{
    return target;
}

inline const Matrix<4, 1> &Camera::cGetPosition() const
{
    return position;
}

inline const Matrix<4, 1> &Camera::cGetTarget() const
{
    return target;
}

inline const Dot &Camera::cGetResolution() const
{
    return resolution;
}

inline Dot &Camera::getResolution()
{
    return resolution;
}

inline const int Camera::getFOV() const
{
    return fov;
}

inline const double Camera::getAspect() const
{
    return ((double)resolution.x) / resolution.y;
}
