//
// Created by Иван Ильин on 19.01.2021.
//

#ifndef ENGINE_PLANE_H
#define ENGINE_PLANE_H

#include "Vec4D.h"
#include "../Triangle.h"

class Plane final {
private:
    const Vec3D _normal;
    const Vec3D _point;
public:
    Plane() = delete;

    Plane(const Plane &plane) = default;

    // You can define plane by defining the points in 3D space
    explicit Plane(const Triangle &tri);

    // Or by defining normal vector and one val laying on the plane
    Plane(const Vec3D &N, const Vec3D &P);

    [[nodiscard]] double distance(const Vec3D &point4D) const;

    // Vec4D in space where line ('start' to 'end') intersects plain with normal vector '_n' and val '_p' lays on the plane
    [[nodiscard]] std::pair<Vec3D, double> intersection(const Vec3D &start, const Vec3D &end) const;

    [[nodiscard]] std::vector<Triangle> clip(const Triangle &tri) const;

    [[nodiscard]] Vec3D N() const { return _normal; }

    [[nodiscard]] Vec3D P() const { return _point; }
};


#endif //INC_3DZAVR_PLANE_H
