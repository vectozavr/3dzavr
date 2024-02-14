//
// Created by Иван Ильин on 19.01.2021.
//
#include <vector>

#include "Plane.h"

Plane::Plane(const Triangle &tri, const ObjectTag& nameTag, const Color& color) :
Object(nameTag), _normal(tri.norm()), _point(tri[0]) {}

Plane::Plane(const Vec3D &N, const Vec3D &P, const ObjectTag& nameTag, const Color& color) :
Object(nameTag), _normal(N.normalized()), _point(P) {}

double Plane::distance(const Vec3D &point) const {
    return point.dot(_normal) - _point.dot(_normal);
}

Plane::Plane(const ObjectTag &tag, const Plane &plane) :
Object(tag, plane), _point(plane._point), _normal(plane._normal) {

}

stack_vector<Triangle, 2> Plane::clip(const Triangle &tri) const {

    stack_vector<Triangle, 2> result;

    // points coordinated
    stack_vector<Vec3D, 3> insidePoints;
    stack_vector<Vec3D, 3> outsidePoints;
    // texture coordinates
    stack_vector<Vec3D, 3> insideTexUV;
    stack_vector<Vec3D, 3> outsideTexUV;
    auto textureCoord = tri.textureCoordinates();

    double distances[3] = {distance(Vec3D(tri[0])),
                           distance(Vec3D(tri[1])),
                           distance(Vec3D(tri[2]))};

    for (int i = 0; i < 3; i++) {
        if (distances[i] >= 0) {
            insidePoints.emplace_back(tri[i]);
            insideTexUV.emplace_back(textureCoord[i]);
        } else {
            outsidePoints.emplace_back(tri[i]);
            outsideTexUV.emplace_back(textureCoord[i]);
        }
    }

    if (insidePoints.size() == 1) {
        auto intersect1 = const_cast<Plane*>(this)->intersect(insidePoints[0], outsidePoints[0]);
        auto intersect2 = const_cast<Plane*>(this)->intersect(insidePoints[0], outsidePoints[1]);

        Vec3D textCoord1 = insideTexUV[0] + (outsideTexUV[0] - insideTexUV[0])*intersect1.k;
        Vec3D textCoord2 = insideTexUV[0] + (outsideTexUV[1] - insideTexUV[0])*intersect2.k;

        std::array<Vec3D, 3> newTexUV{insideTexUV[0], textCoord1, textCoord2};

        result.emplace_back(std::array<Vec4D, 3>{insidePoints[0].makePoint4D(),
                                                 intersect1.pointOfIntersection.makePoint4D(),
                                                 intersect2.pointOfIntersection.makePoint4D()},
                            newTexUV);
    }

    if (insidePoints.size() == 2) {
        auto intersect1 = const_cast<Plane*>(this)->intersect(insidePoints[0], outsidePoints[0]);
        auto intersect2 = const_cast<Plane*>(this)->intersect(insidePoints[1], outsidePoints[0]);

        Vec3D textCoord1 = insideTexUV[0] + (outsideTexUV[0] - insideTexUV[0])*intersect1.k;
        Vec3D textCoord2 = insideTexUV[1] + (outsideTexUV[0] - insideTexUV[1])*intersect2.k;

        std::array<Vec3D, 3> newTexUV1{insideTexUV[0], textCoord1, insideTexUV[1]};
        std::array<Vec3D, 3> newTexUV2{textCoord1, textCoord2, insideTexUV[1]};

        result.emplace_back(std::array<Vec4D, 3>{insidePoints[0].makePoint4D(),
                                                 intersect1.pointOfIntersection.makePoint4D(),
                                                 insidePoints[1].makePoint4D()},
                            newTexUV1);

        result.emplace_back(std::array<Vec4D, 3>{intersect1.pointOfIntersection.makePoint4D(),
                                                 intersect2.pointOfIntersection.makePoint4D(),
                                                 insidePoints[1].makePoint4D()},
                            newTexUV2);
    }

    if (insidePoints.size() == 3) {
        result.emplace_back(tri);
    }

    return result;
}

Object::IntersectionInformation Plane::intersect(const Vec3D &from, const Vec3D &to) {
    double s_dot_n = from.dot(_normal);
    double k = std::numeric_limits<double>::infinity();

    double den = s_dot_n - to.dot(_normal);
    if(std::abs(den) > Consts::EPS) {
        k =  (s_dot_n - _point.dot(_normal)) / den;
    }

    Vec3D point = from + (to - from) * k;
    double distance = (point - from).abs();

    return Object::IntersectionInformation{point,
                                           _normal,
                                           distance,
                                           name(),
                                           shared_from_this(),
                                           (k > 0) && (std::abs(k) < std::numeric_limits<double>::infinity()),
                                           k};
}
