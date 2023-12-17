#include <Vertex.hpp>

Vertex::Vertex() : BaseVertex() {}

Vertex::Vertex(const Vertex &v) = default;

Vertex &Vertex::operator=(const Vertex &v) {
    if (this == &v)
        return *this;

    v1 = v.v1;
    v2 = v.v2;
    v3 = v.v3;
    v4 = v.v4;

    return *this;
}

Vertex::Vertex(const double x,
               const double y,
               const double z,
               const double w)
        : BaseVertex(x, y, z, w) {}

Vertex Vertex::parse(const std::string &line) {
    const auto acc = BaseVertex::parse(line);
    return {acc[0], acc[1], acc[2], acc[3]};
}
