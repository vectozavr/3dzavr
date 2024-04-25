#ifndef GEOMETRY_PLANE_H
#define GEOMETRY_PLANE_H

#include <vector>

#include "linalg/Vec4D.h"
#include "objects/Object.h"
#include "utils/stack_vector.h"
#include "Triangle.h"
#include "Line.h"


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

    [[nodiscard]] double distance(const Vec3D &point3D) const;
    [[nodiscard]] IntersectionInformation intersect(const Vec3D &from, const Vec3D &to) const;
    void clip(std::vector<std::pair<Vec3D, Vec3D>>& input, std::vector<std::pair<Vec3D, Vec3D>>& output) const;
    [[nodiscard]] Line clip(const Line& line, bool& isFullyOutside) const;
};


#endif //GEOMETRY_PLANE_H
