//
// Created by Иван Ильин on 19.01.2021.
//

#ifndef ENGINE_PLANE_H
#define ENGINE_PLANE_H

#include "linalg/Vec4D.h"
#include "Triangle.h"
#include "objects/Object.h"
#include "utils/stack_vector.h"


class Plane final : public Object {
private:
    const Vec3D _normal;
    const Vec3D _point;
public:
    Plane() = delete;

    Plane(const Plane &plane) = default;
    Plane(const ObjectTag& tag, const Plane &plane);

    // You can define plane by defining the points in 3D space
    explicit Plane(const Triangle &tri, const ObjectTag& nameTag, const Color& color = Consts::WHITE_COLORS[2]);

    // Or by defining normal vector and one val laying on the plane
    Plane(const Vec3D &N, const Vec3D &P, const ObjectTag& nameTag, const Color& color = Consts::WHITE_COLORS[2]);

    [[nodiscard]] double distance(const Vec3D &point4D) const;
    [[nodiscard]] IntersectionInformation intersect(const Vec3D &from, const Vec3D &to) override;
    [[nodiscard]] stack_vector<Triangle, 2> clip(const Triangle &tri) const;
    [[nodiscard]] Vec3D N() const { return _normal; }
    [[nodiscard]] Vec3D P() const { return _point; }

    std::shared_ptr<Object> copy(const ObjectTag& tag) const override {
        return std::make_shared<Plane>(tag, *this);
    }
};


#endif //INC_3DZAVR_PLANE_H
