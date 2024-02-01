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
public:
    Sphere(double radius, const Vec3D& position, const ObjectTag& tag);
    Sphere(const ObjectTag& tag, const Sphere& sphere);

    [[nodiscard]] IntersectionInformation intersect(const Vec3D &from, const Vec3D &to) override;

    std::shared_ptr<Object> copy(const ObjectTag& tag) const override {
        return std::make_shared<Sphere>(tag, *this);
    }
};


#endif //RAYTRACE3D_SPHERE_H
