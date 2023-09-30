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

class Dot
{
public:
    Dot(const int p_x, const int p_y);

    int x, y;
};
