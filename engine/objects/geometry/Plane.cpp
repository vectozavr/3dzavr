#include "Plane.h"

Plane::Plane() : normal(1), offset() {}

Plane::Plane(const Triangle &tri) : normal(tri.norm()), offset(tri.norm().dot(Vec3D(tri[0]))) {}

Plane::Plane(const Vec3D &normal, const Vec3D &point) : normal(normal.normalized()), offset(normal.normalized().dot(point)) {}

Plane::Plane(const Vec3D &normal, double offset) : normal(normal.normalized()), offset(offset) {}

double Plane::distance(const Vec3D &v) const {
    return v.dot(normal) - offset;
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

Plane::IntersectionInformation Plane::intersect(const Vec3D &from, const Vec3D &to) const {
    double s_dot_n = from.dot(normal);
    double k = std::numeric_limits<double>::infinity();

    double den = s_dot_n - to.dot(normal);
    bool intersected = false;
    if(std::abs(den) > Consts::EPS) {
        k = (s_dot_n - offset) / den;
        intersected = k > 0;
    }

    Vec3D v = from + (to - from) * k;
    double distance = (v - from).abs();

    return Plane::IntersectionInformation{v, distance, k, intersected};
}
