#pragma once

enum class EntryType {
    Vertex,
    TextureVertex,
    NormalVertex,
    Polygon
};

enum class AxisName {
    X,
    Y,
    Z
};

enum class Direction {
    Forward,
    Backward
};

enum class ModelMatrixConvert {
    Move,
    Rotate,
    Scale
};

enum class SceneMatrixConvert {
    View,
    Projection,
    Viewport
};

enum ShadingModel {
    Flat,
    Phong
};

enum class TextureType {
    Diffuse,
    Normal,
    Specular
};

using uint = unsigned int;
using ubyte = unsigned char;
