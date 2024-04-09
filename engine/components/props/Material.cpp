#include "Material.h"

bool MaterialTag::contains(const MaterialTag &nameTag) const {
    if(_name.find(nameTag.str()) != std::string::npos) {
        return true;
    }
    return false;
}

Material::Material(const MaterialTag &tag, std::shared_ptr<Texture> texture, const Color &ambient, const Color &diffuse,
                   const Color &specular, uint8_t illum, double d):
                   _tag(tag), _texture(texture), _ambient(ambient),
                   _diffuse(diffuse), _specular(specular), _illum(illum), _d(d) {
    checkTransparent();
}

void Material::setAmbient(const Color &color) {
    _ambient = color;
    checkTransparent();
}

void Material::setTransparency(double d) {
    _d = d;
    checkTransparent();
}

void Material::checkTransparent() {
    if(_texture) {
        _isTransparent = _texture->isTransparent() || (_d < 1.0-Consts::EPS);
    } else {
        _isTransparent = (_ambient.a() != 255) || (_d < 1.0-Consts::EPS);
    }
}
