//
// Created by Иван Ильин on 19.01.2021.
//
#include <vector>

#include "Plane.h"

Plane::Plane(const Triangle &tri, const ObjectNameTag& nameTag, const Color& color) :
Object(nameTag), _normal(tri.norm()), _point(tri[0]), _color(color) {}

Plane::Plane(const Vec3D &N, const Vec3D &P, const ObjectNameTag& nameTag, const Color& color) :
Object(nameTag), _normal(N.normalized()), _point(P), _color(color) {}

double Plane::distance(const Vec3D &point) const {
    return point.dot(_normal) - _point.dot(_normal);
}

std::vector<Triangle> Plane::clip(const Triangle &tri) const {

    std::vector<Triangle> result;

    // points coordinated
    std::vector<Vec3D> insidePoints;
    std::vector<Vec3D> outsidePoints;
    // texture coordinates
    std::vector<Vec3D> insideTexUV;
    std::vector<Vec3D> outsideTexUV;
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
        auto intersect1 = intersect(insidePoints[0], outsidePoints[0]);
        auto intersect2 = intersect(insidePoints[0], outsidePoints[1]);

        Vec3D textCoord1 = insideTexUV[0] + (outsideTexUV[0] - insideTexUV[0])*intersect1.k;
        Vec3D textCoord2 = insideTexUV[0] + (outsideTexUV[1] - insideTexUV[0])*intersect2.k;

        std::array<Vec3D, 3> newTexUV{insideTexUV[0], textCoord1, textCoord2};

        result.emplace_back(std::array<Vec4D, 3>{insidePoints[0].makePoint4D(),
                                                 intersect1.pointOfIntersection.makePoint4D(),
                                                 intersect2.pointOfIntersection.makePoint4D()},
                            newTexUV,tri.colors());
    }

    if (insidePoints.size() == 2) {
        auto intersect1 = intersect(insidePoints[0], outsidePoints[0]);
        auto intersect2 = intersect(insidePoints[1], outsidePoints[0]);

        Vec3D textCoord1 = insideTexUV[0] + (outsideTexUV[0] - insideTexUV[0])*intersect1.k;
        Vec3D textCoord2 = insideTexUV[1] + (outsideTexUV[0] - insideTexUV[1])*intersect2.k;

        std::array<Vec3D, 3> newTexUV1{insideTexUV[0], textCoord1, insideTexUV[1]};
        std::array<Vec3D, 3> newTexUV2{textCoord1, textCoord2, insideTexUV[1]};

        result.emplace_back(std::array<Vec4D, 3>{insidePoints[0].makePoint4D(),
                                                 intersect1.pointOfIntersection.makePoint4D(),
                                                 insidePoints[1].makePoint4D()},
                            newTexUV1, tri.colors());

        result.emplace_back(std::array<Vec4D, 3>{intersect1.pointOfIntersection.makePoint4D(),
                                                 intersect2.pointOfIntersection.makePoint4D(),
                                                 insidePoints[1].makePoint4D()},
                            newTexUV2, tri.colors());
    }

    if (insidePoints.size() == 3) {
        result.emplace_back(tri);
    }

    return result;
}

Object::IntersectionInformation Plane::intersect(const Vec3D &from, const Vec3D &to) const {
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
                                           std::make_shared<Object>(*this),
                                           (k > 0) && (std::abs(k) < std::numeric_limits<double>::infinity()),
                                           k,
                                           _color};
}
