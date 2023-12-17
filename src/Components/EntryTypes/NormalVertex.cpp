#include <NormalVertex.hpp>

NormalVertex::NormalVertex(const double i, const double j, const double k)
        : BaseVertex(i, j, k) {}

NormalVertex NormalVertex::parse(const std::string &line) {
    const auto acc = BaseVertex::parse(line);
    return {acc[0], acc[1], acc[2]};
}
