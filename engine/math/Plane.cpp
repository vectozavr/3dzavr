//
// Created by Иван Ильин on 19.01.2021.
//

#include "Plane.h"

Plane::Plane(const Triangle &tri) : _normal(tri.norm()), _point(tri[0]) {
}

Plane::Plane(const Vec3D &N, const Vec3D &P) : _normal(N.normalized()), _point(P) {
}

double Plane::distance(const Vec3D &point) const {
    return point.dot(_normal) - _point.dot(_normal);
}

std::pair<Vec3D, double> Plane::intersection(const Vec3D &start, const Vec3D &end) const {
    double s_dot_n = start.dot(_normal);
    double k = (s_dot_n - _point.dot(_normal)) / (s_dot_n - end.dot(_normal));
    Vec3D res = start + (end - start) * k;
    return std::make_pair(res, k);
}

std::vector<Triangle> Plane::clip(const Triangle &tri) const {

    std::vector<Triangle> result;

    std::vector<Vec3D> insidePoints;
    std::vector<Vec3D> outsidePoints;

    double distances[3] = {distance(Vec3D(tri[0])),
                           distance(Vec3D(tri[1])),
                           distance(Vec3D(tri[2]))};

    for (int i = 0; i < 3; i++) {
        if (distances[i] >= 0) {
            insidePoints.emplace_back(tri[i]);
        } else {
            outsidePoints.emplace_back(tri[i]);
        }
    }

    if (insidePoints.size() == 1) {
        std::pair<Vec3D, double> intersect1 = intersection(insidePoints[0], outsidePoints[0]);
        std::pair<Vec3D, double> intersect2 = intersection(insidePoints[0], outsidePoints[1]);

        result.emplace_back(insidePoints[0].makePoint4D(),
                            intersect1.first.makePoint4D(),
                            intersect2.first.makePoint4D(),
                            tri.color());
    }

    if (insidePoints.size() == 2) {
        std::pair<Vec3D, double> intersect1 = intersection(insidePoints[0], outsidePoints[0]);
        std::pair<Vec3D, double> intersect2 = intersection(insidePoints[1], outsidePoints[0]);

        result.emplace_back(insidePoints[0].makePoint4D(),
                            intersect1.first.makePoint4D(),
                            insidePoints[1].makePoint4D(),
                            tri.color());
        result.emplace_back(intersect1.first.makePoint4D(),
                            intersect2.first.makePoint4D(),
                            insidePoints[1].makePoint4D(),
                            tri.color());
    }

    if (insidePoints.size() == 3) {
        result.emplace_back(tri);
    }

    return result;
}
