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
    if(texture) {
        _isTransparent = texture->isTransparent() || (d < 1.0-Consts::EPS);
    } else {
        _isTransparent = (ambient.a() != 255) || (d < 1.0-Consts::EPS);
    }
}

void Material::setAmbient(const Color &color) {
    _ambient = color;
    _isTransparent = (_ambient.a() != 255) || (_d < 1.0-Consts::EPS);
}

void Material::setTransparency(double d) {
    _d = d;
    _isTransparent = (_ambient.a() != 255) || (d < 1.0-Consts::EPS);
}
