//
// Created by Иван Ильин on 19.01.2021.
//

#ifndef ENGINE_PLANE_H
#define ENGINE_PLANE_H

#include "linalg/Vec4D.h"
#include "Triangle.h"
#include "objects/Object.h"
#include "utils/stack_vector.h"


class Plane final {
private:
    struct IntersectionInformation final {
        Vec3D pointOfIntersection;
        double distanceToObject = std::numeric_limits<double>::infinity();
        double k = 0;
        bool intersected = false;
    };
public:
    Vec3D normal;
    double offset;

    Plane();
    Plane(const Plane &plane) = default;

    // You can define plane by defining the points in 3D space
    explicit Plane(const Triangle &tri);

    // Or by defining normal vector and one val laying on the plane
    Plane(const Vec3D &normal, const Vec3D &point);
    Plane(const Vec3D &normal, double offset);

    Plane& operator=(const Plane &plane) = default;

    [[nodiscard]] double distance(const Vec3D &point4D) const;
    [[nodiscard]] IntersectionInformation intersect(const Vec3D &from, const Vec3D &to) const;
    [[nodiscard]] stack_vector<Triangle, 2> clip(const Triangle &tri) const;
};


#endif //ENGINE_PLANE_H
