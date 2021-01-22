//
// Created by Иван Ильин on 19.01.2021.
//

#ifndef INC_3DZAVR_PLANE_H
#define INC_3DZAVR_PLANE_H

#include "utils/Point4D.h"
#include "Triangle.h"

class Plane {
private:
    // You can define plane by defining the points in 3D space
    Triangle triangle;
    // Or by defining normal vector and one point laying on the plane
    Point4D n = {0, 0, 1, 0};
    Point4D p{};
public:
    // A plain with normal vector 'n' and point 'p' lays on the plane
    Plane() = default;
    Plane(const Point4D& N, const Point4D& P);
    Plane(const Plane& plane);
    explicit Plane(const Triangle& tri);

    [[nodiscard]] double distance(const Point4D& point4D) const;
    // Point4D in space where line ('start' to 'end') intersects plain with normal vector 'n' and point 'p' lays on the plane
    Point4D intersection(const Point4D& start, const Point4D& end);
    int clip(Triangle& tri, Triangle& additional_tri);

    [[nodiscard]] Point4D N() const { return n; }
    [[nodiscard]] Point4D P() const { return p; }
};


#endif //INC_3DZAVR_PLANE_H
