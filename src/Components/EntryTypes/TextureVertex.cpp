#include <TextureVertex.hpp>

TextureVertex::TextureVertex(const double u, const double v, const double w)
        : BaseVertex(u, v, w) {}

TextureVertex TextureVertex::parse(const std::string &line) {
    const auto acc = BaseVertex::parse(line);
    return TextureVertex{acc[0], acc[1], acc[2]};
}
