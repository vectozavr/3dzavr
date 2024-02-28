//
// Created by Иван Ильин on 13.01.2021.
//

#include "Triangle.h"
#include "Consts.h"

Triangle::Triangle(const std::array<Vec4D, 3>& p, const std::array<Vec3D, 3>& uv) :
    _points{p}, _textureCoordinates(uv) {
    calculateNormal();
}

void Triangle::calculateNormal() {
    auto v1 = Vec3D(_points[1] - _points[0]);
    auto v2 = Vec3D(_points[2] - _points[0]);
    Vec3D crossProduct = v1.cross(v2);

    if (crossProduct.sqrAbs() > Consts::EPS) {
        _normal = crossProduct.normalized();
    } else {
        _normal = Vec3D(0);
    }
}

Triangle Triangle::operator*(const Matrix4x4 &matrix4X4) const {
    return Triangle{{matrix4X4 * _points[0], matrix4X4 * _points[1], matrix4X4 * _points[2]}, _textureCoordinates};
}

bool Triangle::isPointInside(const Vec3D &point) const {

    // TODO: this can be made more efficiently: by finding alpha betta gamma for this triangle and the point.
    Vec3D triangleNorm = norm();
    double dot1 = (point - Vec3D(_points[0])).cross(Vec3D(_points[1] - _points[0])).dot(triangleNorm);
    double dot2 = (point - Vec3D(_points[1])).cross(Vec3D(_points[2] - _points[1])).dot(triangleNorm);
    double dot3 = (point - Vec3D(_points[2])).cross(Vec3D(_points[0] - _points[2])).dot(triangleNorm);

    if ((dot1 >= 0 && dot2 >= 0 && dot3 >= 0) || (dot1 <= 0 && dot2 <= 0 && dot3 <= 0)) {
        return true;
    }
    return false;
}

Vec3D Triangle::abgBarycCoord(const Vec2D& point) const {
    Vec2D ba = Vec2D(_points[1]) - Vec2D(_points[0]);
    Vec2D ca = Vec2D(_points[2]) - Vec2D(_points[0]);
    auto pa = point - Vec2D(_points[0]);

    bool swapped = std::abs(ca.y()) < Consts::EPS;
    if (swapped) {
        std::swap(ba, ca);
    }

    double betta = (pa.y() * ca.x() - pa.x() * ca.y()) /
                   (ba.y() * ca.x() - ba.x() * ca.y());
    double gamma = (pa.y() - betta * ba.y()) / ca.y();
    double alpha = 1.0 - betta - gamma;

    if (swapped) {
        std::swap(betta, gamma);
    }

    return Vec3D{alpha, betta, gamma};
}

Vec2D Triangle::uvCoord(const Vec2D &point) const {
    return {};
}

Vec2D Triangle::uvCoordDer(const Vec2D &point) const {
    // TODO: implement or delete
    return {};
}
