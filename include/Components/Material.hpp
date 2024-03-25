#pragma once

#include <SFML/Graphics.hpp>
#include <Texture.hpp>
#include <Matrix.hpp>
#include <memory>

class Material
{
public:
    Material(
        const std::string &_name,
        const std::optional<Vector<4>> &_ambient,
        const std::optional<Vector<4>> &_diffuse,
        const std::optional<Vector<4>> &_specular,
        const std::optional<double> _specularExp,
        std::shared_ptr<const Texture> _diffuseMap,
        std::shared_ptr<const Texture> _emissiveMap,
        std::shared_ptr<const Texture> _normalMap,
        std::shared_ptr<const Texture> _mraoMap);

    const std::shared_ptr<const Texture> cGetDiffuseMap() const;
    const std::shared_ptr<const Texture> cGetEmissiveMap() const;
    const std::shared_ptr<const Texture> cGetNormalMap() const;
    const std::shared_ptr<const Texture> cGetMRAOMap() const;

    const std::string &cGetName() const;
    const std::optional<Vector<4>> cGetDiffuseCoeff() const;
    const std::optional<Vector<4>> cGetAmbientCoeff() const;
    const std::optional<Vector<4>> cGetSpecularCoeff() const;
    const std::optional<double> cGetSpecularExp() const;

private:
    std::string name;
    std::optional<Vector<4>> ambient;
    std::optional<Vector<4>> diffuse;
    std::optional<Vector<4>> specular;
    std::optional<double> specularExp;
    std::shared_ptr<const Texture> diffuseMap;
    std::shared_ptr<const Texture> emissiveMap;
    std::shared_ptr<const Texture> normalMap;
    std::shared_ptr<const Texture> mraoMap;
};

inline const std::shared_ptr<const Texture> Material::cGetDiffuseMap() const
{
    return diffuseMap;
}

inline const std::shared_ptr<const Texture> Material::cGetEmissiveMap() const
{
    return emissiveMap;
}

inline const std::shared_ptr<const Texture> Material::cGetNormalMap() const
{
    return normalMap;
}

inline const std::shared_ptr<const Texture> Material::cGetMRAOMap() const
{
    return mraoMap;
}

inline const std::string &Material::cGetName() const
{
    return name;
}

inline const std::optional<Vector<4>> Material::cGetDiffuseCoeff() const
{
    return diffuse;
}

inline const std::optional<Vector<4>> Material::cGetAmbientCoeff() const
{
    return ambient;
}

inline const std::optional<Vector<4>> Material::cGetSpecularCoeff() const
{
    return specular;
}

inline const std::optional<double> Material::cGetSpecularExp() const
{
    return specularExp;
}
