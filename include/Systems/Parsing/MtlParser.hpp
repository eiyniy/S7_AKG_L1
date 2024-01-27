#pragma once

#include <string>
#include <memory>
#include <Material.hpp>
#include <Matrix.hpp>
#include <BaseTextParser.hpp>
#include <Types.hpp>

class MtlParser : public BaseTextParser
{
public:
    explicit MtlParser(const std::string &_pathToMtl);

    std::unique_ptr<const std::map<std::string, std::shared_ptr<const Material>>> parse();

private:
    std::unique_ptr<std::map<std::string, std::shared_ptr<const Material>>> materials;

    std::string name;
    std::optional<Matrix<4, 1>> ambient;
    std::optional<Matrix<4, 1>> diffuse;
    std::optional<Matrix<4, 1>> specular;
    std::optional<double> specularExp;
    std::unique_ptr<const Texture> diffuseMap;
    std::unique_ptr<const Texture> emissiveMap;
    std::unique_ptr<const Texture> normalMap;
    std::unique_ptr<const Texture> mraoMap;

    void parseEntry(const std::string &line);

    static std::optional<MtlEntryType> getEntryType(const std::string &line);

    void resetMaterial();

    static const Matrix<4, 1> parseCoeff(const std::string &line);
    static std::unique_ptr<const Texture> parseTexture(
        const std::string &line,
        const std::string &pathToFile,
        std::optional<MtlEntryType> optType = std::nullopt);
};
