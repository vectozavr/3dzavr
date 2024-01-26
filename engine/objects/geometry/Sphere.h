//
// Created by Ivan Ilin on 07/10/2023.
//

#ifndef RAYTRACE3D_SPHERE_H
#define RAYTRACE3D_SPHERE_H

#include "objects/Object.h"
#include "objects/props/Color.h"
#include "Consts.h"

class Sphere final : public Object {
private:
    double _radius = 0;
    Color _color;
public:
    Sphere(double radius, const Vec3D& position, const ObjectTag& nameTag, const Color& color = Consts::WHITE_COLORS[0]);

    [[nodiscard]] IntersectionInformation intersect(const Vec3D &from, const Vec3D &to) const override;
};


#endif //RAYTRACE3D_SPHERE_H
