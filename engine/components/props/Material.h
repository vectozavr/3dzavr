#ifndef PROPS_MATERIAL_H
#define PROPS_MATERIAL_H

#include <string>

#include "linalg/Vec3D.h"
#include "Texture.h"

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

    Color _ambient, _diffuse, _specular;
    uint16_t _illum;
    double _d = 1.0;

    bool _isTransparent = false;
public:
    Material(const MaterialTag& tag,
             std::shared_ptr<Texture> texture,
             const Color& ambient = Color::WHITE,
             const Color& diffuse = Color::WHITE,
             const Color& specular = Color::WHITE,
             uint8_t illum = 1,
             double d = 1);

    [[nodiscard]] Color ambient() const { return _ambient; }
    [[nodiscard]] Color diffuse() const { return _diffuse; }
    [[nodiscard]] Color specular() const { return _specular; }
    [[nodiscard]] uint8_t illum() const { return _illum; }
    [[nodiscard]] double d() const { return _d; }

    void setAmbient(const Color& color);
    void setTransparency(double d);

    [[nodiscard]] std::shared_ptr<Texture> texture() const {return _texture; }

    [[nodiscard]] MaterialTag tag() const { return _tag; }

    [[nodiscard]] bool isTransparent() const {return _isTransparent; }
};


#endif //PROPS_MATERIAL_H
