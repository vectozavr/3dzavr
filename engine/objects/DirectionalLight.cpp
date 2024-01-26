//
// Created by Ivan Ilin on 18/10/2023.
//

#include "DirectionalLight.h"

DirectionalLight::DirectionalLight(const ObjectTag& tag, const Vec3D &direction, const Color &color) :
Object(tag), _dir(direction.normalized()), _color(color) {

}

Color DirectionalLight::color() const {
    return _color;
}

Vec3D DirectionalLight::direction() const {
    return _dir;
}
