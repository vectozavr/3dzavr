//
// Created by Ivan Ilin on 18/10/2023.
//

#ifndef RAYTRACE3D_DIRECTIONALLIGHT_H
#define RAYTRACE3D_DIRECTIONALLIGHT_H

#include <objects/Object.h>

class DirectionalLight final : public Object {
private:
    Vec3D _dir;
    Color _color;
public:
    DirectionalLight(const ObjectTag& tag, const Vec3D& direction, const Color& color = Consts::WHITE);

    [[nodiscard]] Color color() const;
    [[nodiscard]] Vec3D direction() const;
};


#endif //RAYTRACE3D_DIRECTIONALLIGHT_H
