#include <TextureVertex.hpp>

TextureVertex::TextureVertex(const double u, const double v, const double w)
        : BaseVertex(u, v, w) {}

TextureVertex TextureVertex::parse(const std::string &line) {
    auto acc = BaseVertex::parse(line);
    return TextureVertex(acc[0], acc[1], acc[2]);
}

const double TextureVertex::getU() const {
    return v1;
}

const double TextureVertex::getV() const {
    return v2;
}

const double TextureVertex::getW() const {
    return v3;
}
