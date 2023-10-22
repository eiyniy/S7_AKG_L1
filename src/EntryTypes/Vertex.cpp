#include <Vertex.hpp>
#include <ObjParser.hpp>

Vertex::Vertex()
    : BaseVertex(),
      isVisible(true) {}

Vertex::Vertex(const Vertex &v)
    : BaseVertex(v),
      isVisible(v.isVisible) {}

Vertex &Vertex::operator=(const Vertex &v)
{
    if (this == &v)
        return *this;

    v1 = v.v1;
    v2 = v.v2;
    v3 = v.v3;
    v4 = v.v4;
    isVisible = v.isVisible;

    return *this;
}

Vertex::Vertex(const double x,
               const double y,
               const double z,
               const double w,
               const bool _isVisible)
    : BaseVertex(x, y, z, w),
      isVisible(_isVisible) {}

Vertex Vertex::parse(const std::string &line)
{
    auto acc = BaseVertex::parse(line);
    return Vertex(acc[0], acc[1], acc[2], acc[3]);
}

const bool Vertex::IsVisible() const
{
    return isVisible;
}
