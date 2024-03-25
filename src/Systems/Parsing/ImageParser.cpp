#include <ImageParser.hpp>
#include <Globals.hpp>

ImageParser::ImageParser(const std::string &_path, const TextureType _type)
    : path(_path), type(_type) {}

std::unique_ptr<const Texture> ImageParser::parse() const
{
    std::vector<Vector<4>> data;

    const cimg::CImg<ubyte> image = cimg::CImg<>(path.c_str());
    // image.display();

    uint width = image.width();
    uint height = image.height();

    data = std::vector<Vector<4>>{width * height};

#pragma omp parallel for if (!_DEBUG)
    for (uint i = 0; i < height; ++i)
    {
        for (uint j = 0; j < width; ++j)
        {
            const auto r = image(j, i, 0);
            const auto g = image(j, i, 1);
            const auto b = image(j, i, 2);

            if (type == TextureType::Diffuse || type == TextureType::Emissive)
                data.at(j + width * i) = Vector<4>(r, g, b, 0);
            else if (type == TextureType::Normal)
            {
                data.at(j + width * i) = Vector<4>(
                    r / 255.0 * 2 - 1,
                    g / 255.0 * 2 - 1,
                    b / 255.0 * 2 - 1,
                    0);
                data.at(j + width * i).normalize();
            }
            else if (type == TextureType::MRAO)
            {
                data.at(j + width * i) = Vector<4>(
                    r / 255.0,
                    g / 255.0,
                    b / 255.0,
                    0);
                data.at(j + width * i).normalize();
            }
        }
    }

    return std::make_unique<const Texture>(width, height, data);
}
