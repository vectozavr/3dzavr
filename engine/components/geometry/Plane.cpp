#include "Plane.h"

Plane::Plane() : normal(1), offset() {}

Plane::Plane(const Triangle &tri) : normal(tri.norm()), offset(tri.norm().dot(Vec3D(tri[0]))), point(Vec3D(tri[0])) {}

Plane::Plane(const Vec3D &normal, const Vec3D &point) : normal(normal.normalized()), point(point), offset(normal.normalized().dot(point)) {}

Plane::Plane(const Vec3D &normal, double offset) : normal(normal.normalized()), offset(offset) {
    point = normal*offset;
}

double Plane::distance(const Vec3D &v) const {
    return v.dot(normal) - offset;
}

void Plane::clip(std::vector<std::pair<Vec3D, Vec3D>>& input, std::vector<std::pair<Vec3D, Vec3D>>& output) const {
    if (input.empty()) return;
    auto prev = input.back();
    double prevDistance = distance(prev.first);
    for (auto &curr : input) {
        double currDistance = distance(curr.first);

        if (currDistance >= 0) {
            if (prevDistance < 0) {
                auto p = intersect(prev.first, curr.first);
                output.emplace_back(p.pointOfIntersection, prev.second + (curr.second - prev.second) * p.k);
            }
            output.emplace_back(curr);
        } else if (prevDistance >= 0) {
            auto p = intersect(prev.first, curr.first);
            output.emplace_back(p.pointOfIntersection, prev.second + (curr.second - prev.second) * p.k);
        }
        prev = curr;
        prevDistance = currDistance;
    }
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

Line Plane::clip(const Line &line, bool& isFullyOutside) const {
    double distances[2] = {distance(Vec3D(line.p1())),
                           distance(Vec3D(line.p2()))};

    isFullyOutside = false;
    if((distances[0] < 0) && (distances[1] < 0)) {
        // In this case the line is fully outside the clipping plane
        isFullyOutside = true;
        return {};
    }
    if((distances[0] >= 0) && (distances[1] >= 0)) {
        // In this case the line is fully inside the clipping plane
        return line;
    }
    if((distances[0] < 0) && (distances[1] >= 0)) {
        // The first point is outside
        auto intersection = intersect(Vec3D(line.p1()), Vec3D(line.p2()));
        return {intersection.pointOfIntersection.makePoint4D(), line.p2()};
    }
    if((distances[0] >= 0) && (distances[1] < 0)) {
        // The second point is outside
        auto intersection = intersect(Vec3D(line.p1()), Vec3D(line.p2()));
        return {line.p1(), intersection.pointOfIntersection.makePoint4D()};
    }

    return {};
}
