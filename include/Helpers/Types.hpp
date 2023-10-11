#pragma once

enum class EntryType
{
    Vertex,
    TextureVertex,
    NormalVertex,
    Polygon
};

enum class StorageMode
{
    Static,
    Dynamic
};

enum class AxisName
{
    X,
    Y,
    Z
};

enum class Direction
{
    Forward,
    Backward
};

class Dot
{
public:
    Dot(const int p_x, const int p_y);

    int x, y;
};

class SphericalCoordinate
{
public:
    SphericalCoordinate(
        const double p_r,
        const double p_a,
        const double p_b);

    void move(
        const AxisName axisName,
        const Direction direction,
        const double step,
        bool &isCameraReversed);

    double r, a, b;
};
