#pragma once

enum class EntryType {
    Vertex,
    TextureVertex,
    NormalVertex,
    Polygon
};

enum class StorageMode {
    Static,
    Dynamic
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
    Fong
};
