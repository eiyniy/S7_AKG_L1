#pragma once

#include <BaseVertex.hpp>
#include <string>

class TextureVertex : public BaseVertex {
public:
    explicit TextureVertex(double u, double v = 0, double w = 0);

    static TextureVertex parse(const std::string &line);

    [[nodiscard]] double cGetU() const;

    [[nodiscard]] double cGetV() const;

    [[nodiscard]] double cGetW() const;
};

inline double TextureVertex::cGetU() const {
    return v1;
}

inline double TextureVertex::cGetV() const {
    return v2;
}

inline double TextureVertex::cGetW() const {
    return v3;
}
