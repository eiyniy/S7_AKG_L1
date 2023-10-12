#include <Vertex.hpp>
#include <ObjParser.hpp>

Vertex::Vertex()
    : BaseVertex(),
      isOutOfScreen(false) {}

Vertex::Vertex(const Vertex &v)
    : BaseVertex(v),
      isOutOfScreen(v.isOutOfScreen) {}

Vertex &Vertex::operator=(const Vertex &v)
{
    if (this == &v)
        return *this;

    v1 = v.v1;
    v2 = v.v2;
    v3 = v.v3;
    v4 = v.v4;
    isOutOfScreen = v.isOutOfScreen;

    return *this;
}

Vertex::Vertex(const double x,
               const double y,
               const double z,
               const double w,
               const bool p_isOutOfScreen)
    : BaseVertex(x, y, z, w),
      isOutOfScreen(p_isOutOfScreen) {}

Vertex Vertex::parse(const std::string &line)
{
    auto acc = BaseVertex::parse(line);
    return Vertex(acc[0], acc[1], acc[2], acc[3]);
}

const bool Vertex::cGetIsOutOfScreen() const
{
    return isOutOfScreen;
}
