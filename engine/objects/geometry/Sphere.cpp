#include <cmath>

#include "Sphere.h"

Sphere::Sphere(double radius, const Vec3D &position, const ObjectTag& tag)
: Object(tag), _radius(radius) {
    translateToPoint(position);
}

Object::IntersectionInformation Sphere::intersect(const Vec3D &from, const Vec3D &to) {
    double k = std::numeric_limits<double>::infinity();
    Vec3D norm{};

    Matrix4x4 toModelMatrix = fullInvModel();

    Vec3D from_M = Vec3D(toModelMatrix*from.makePoint4D());
    Vec3D to_M = Vec3D(toModelMatrix*to.makePoint4D());

    // This should be done in the coordinate system of the object
    Vec3D d = (to_M-from_M).normalized();
    Vec3D e_c = from_M;

    double d_ec = d.dot(e_c);
    double D = std::pow(d_ec, 2) - d.sqrAbs()*(e_c.sqrAbs()-_radius*_radius);
    if(D >= 0) {
        k = (-d_ec - std::sqrt(D))/d.dot(d);
        norm = (from_M + d*k)/_radius;
    }

    Vec3D point_M = from_M + d*k;

    Matrix4x4 fromModelMatrix = fullModel();

    Vec3D point = Vec3D(fromModelMatrix*point_M.makePoint4D());
    double distance = (point - from).abs();
    norm = Vec3D(fromModelMatrix*norm.makePoint4D()).normalized();

    IntersectionInformation res = Object::IntersectionInformation{
        point,
        norm,
        distance,
        shared_from_this(),
        (k > 0) && (std::abs(k) < std::numeric_limits<double>::infinity())
    };

    return res;
}

Sphere::Sphere(const ObjectTag &tag, const Sphere &sphere) : Object(tag, sphere), _radius(sphere._radius) {

}
