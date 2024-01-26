//
// Created by Иван Ильин on 22/01/2024.
//

#ifndef INC_3DZAVR_TEST_MATERIAL_H
#define INC_3DZAVR_TEST_MATERIAL_H

#include <string>

#include <linalg/Vec3D.h>
#include <objects/props/Texture.h>

class MaterialTag final {
private:
    const std::string _name;
public:
    explicit MaterialTag(std::string name = "") : _name(std::move(name)) {}

    [[nodiscard]] std::string str() const { return _name; }

    bool operator==(const MaterialTag &tag) const { return _name == tag._name; }
    bool operator!=(const MaterialTag &tag) const { return _name != tag._name; }
    bool operator<(const MaterialTag &tag) const { return _name < tag._name; }

    [[nodiscard]] bool contains(const MaterialTag& tag) const;
};

class Material {
private:
    const MaterialTag _tag;
    std::shared_ptr<Texture> _texture = nullptr;
    // TODO: add other maps like normal map, shadow map and eth..
    // std::shared_ptr<Texture> _normalMap = nullptr;
    // std::shared_ptr<Texture> _shadowMap = nullptr;
    // std::shared_ptr<Texture> _diffusionMap = nullptr;
    // ...

    // TODO: maybe these vectors should be of type Color
    Vec3D _ambient, _diffuse, _specular;
    u_int16_t _illum;

public:
    Material(const MaterialTag& tag,
             std::shared_ptr<Texture> texture,
             const Vec3D& ambient,
             const Vec3D& diffuse,
             const Vec3D& specular,
             u_int8_t illum) :
             _tag(tag), _texture(texture), _ambient(ambient), _diffuse(diffuse),
             _specular(specular), _illum(illum) {};

    [[nodiscard]] Vec3D ambient() const { return _ambient; }
    [[nodiscard]] Vec3D diffuse() const { return _diffuse; }
    [[nodiscard]] Vec3D specular() const { return _specular; }
    [[nodiscard]] u_int8_t illum() const { return _illum; }

    [[nodiscard]] std::shared_ptr<Texture> texture() const {return _texture;}

    [[nodiscard]] MaterialTag tag() const { return _tag; }
};


#endif //INC_3DZAVR_TEST_MATERIAL_H
