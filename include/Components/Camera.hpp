#pragma once

#include <Point.hpp>
#include <Matrix.hpp>

class Camera {
private:
    Vector<4> up;
    Vector<4> position;
    Vector<4> target;
    Point resolution;

    const int fov;

public:
    Camera(
            const Vector<4> &_up,
            const Vector<4> &_position,
            const Vector<4> &_target,
            Point &_resolution,
            int _fov);

    void move(const Vector<4> &transition);

    void rotateAround(
            AxisName axisName,
            Direction direction,
            double step);

    void setResolution(const Point &newResolution);

    void setTarget(const Vector<4> &newTarget);

    const Vector<4> &cGetUp() const;

    const Vector<4> &cGetPosition() const;

    const Vector<4> &cGetTarget() const;

    const Point &cGetResolution() const;

    int cGetFOV() const;

    double cGetAspect() const;
};

inline const Vector<4> &Camera::cGetUp() const {
    return up;
}

inline const Vector<4> &Camera::cGetPosition() const {
    return position;
}

inline const Vector<4> &Camera::cGetTarget() const {
    return target;
}

inline const Point &Camera::cGetResolution() const {
    return resolution;
}

inline int Camera::cGetFOV() const {
    return fov;
}

inline double Camera::cGetAspect() const {
    return ((double) resolution.cGetX()) / resolution.cGetY();
}
