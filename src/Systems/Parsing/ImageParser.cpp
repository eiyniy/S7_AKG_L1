#include <ImageParser.hpp>
#include <Globals.hpp>

template <typename T>
ImageParser<T>::ImageParser(const std::string &_path)
    : path(_path)
{
}

template <typename T>
Texture<T> ImageParser<T>::parse() const
{
    std::vector<T> data;

    const cimg::CImg<ubyte> image = cimg::CImg<>(path.c_str());
    // image.display();

    uint width = image.width();
    uint height = image.height();

    data = std::vector<T>{width * height};

#pragma omp parallel for if (!_DEBUG)
    for (uint i = 0; i < height; ++i)
    {
        for (uint j = 0; j < width; ++j)
        {
            const auto r = image(j, i, 0);
            const auto g = image(j, i, 1);
            const auto b = image(j, i, 2);

            if constexpr (std::is_same<T, sf::Color>::value)

                data.at(j + width * i) = sf::Color(r, g, b);
            else if constexpr (std::is_same<T, Matrix<4, 1>>::value)
            {
                data.at(j + width * i) = Matrix<4, 1>(
                    r / 255.0 * 2 - 1,
                    g / 255.0 * 2 - 1,
                    b / 255.0 * 2 - 1,
                    0);
                data.at(j + width * i).normalize();
            }
            else
                throw std::runtime_error("Invalid template type");
        }
    }

    return Texture<T>(width, height, data);
}

template class ImageParser<Matrix<4, 1>>;
template class ImageParser<sf::Color>;
