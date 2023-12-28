#pragma once

#include <cstdint>

enum class EntryType
{
    Vertex,
    TextureVertex,
    NormalVertex,
    Polygon
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

enum class ModelMatrixConvert
{
    Move,
    Rotate,
    Scale
};

enum class SceneMatrixConvert
{
    View,
    Projection,
    Viewport
};

enum ShadingModel
{
    Flat,
    Phong
};

enum class TextureType
{
    Diffuse,
    Normal,
    Emissive,
    MRAO
};

using uint = unsigned int;
using ushort = unsigned short;
using ubyte = unsigned char;
