#include <set>
#include <cmath>

#include "HitBox.h"
#include "Consts.h"

bool HitBox::Vec3DLess::operator()(const Vec3D& lhs, const Vec3D& rhs) const noexcept {
    if (fabs(lhs.x() - rhs.x()) >= Consts::EPS)
        return lhs.x() < rhs.x();
    else if (fabs(lhs.y() - rhs.y()) >= Consts::EPS)
        return lhs.y() < rhs.y();
    else if (fabs(lhs.z() - rhs.z()) >= Consts::EPS)
        return lhs.z() < rhs.z();
    else
        return false;
}

HitBox::HitBox(const TriangleMesh& triangleMesh, bool useSimpleBox) {
    if (useSimpleBox) {
        generateSimpleFromTriangleMesh(triangleMesh);
    } else {
        generateDetailedFromTriangleMesh(triangleMesh);
    }
}

HitBox::HitBox(const LineMesh &lineMesh, bool useSimpleBox) {
    if (useSimpleBox) {
        generateSimpleFromLineMesh(lineMesh);
    } else {
        generateDetailedFromLineMesh(lineMesh);
    }
}

void HitBox::generateSimpleFromTriangleMesh(const TriangleMesh &triangleMesh) {
    double maxX = -std::numeric_limits<double>::max();
    double maxY = -std::numeric_limits<double>::max();
    double maxZ = -std::numeric_limits<double>::max();

    double minX = std::numeric_limits<double>::max();
    double minY = std::numeric_limits<double>::max();
    double minZ = std::numeric_limits<double>::max();

    for(const auto& t : triangleMesh.triangles()) {
        for(int i = 0; i < 3; i++) {
            maxX = std::max(maxX, t[i].x());
            maxY = std::max(maxY, t[i].y());
            maxZ = std::max(maxZ, t[i].z());

            minX = std::min(minX, t[i].x());
            minY = std::min(minY, t[i].y());
            minZ = std::min(minZ, t[i].z());
        }
    }

    // Boxed hitbox out of 8 points
    constructFrom2Points(Vec3D(minX, minY, minZ), Vec3D(maxX, maxY, maxZ));
}

void HitBox::generateDetailedFromTriangleMesh(const TriangleMesh &triangleMesh) {
    // we do not need to add the same points in hit box
    std::set<Vec3D, HitBox::Vec3DLess> points;

    for (const auto& t : triangleMesh.triangles())
        for (int i = 0; i < 3; i++)
            points.insert(Vec3D(t[i]));

    _hitBox.reserve(points.size());
    for (const auto& it : points)
        _hitBox.push_back(it);
    _hitBox.shrink_to_fit();
}

void HitBox::generateSimpleFromLineMesh(const LineMesh &lineMesh) {
    double maxX = -std::numeric_limits<double>::max();
    double maxY = -std::numeric_limits<double>::max();
    double maxZ = -std::numeric_limits<double>::max();

    double minX = std::numeric_limits<double>::max();
    double minY = std::numeric_limits<double>::max();
    double minZ = std::numeric_limits<double>::max();

    for(const auto& line : lineMesh.lines()) {
        maxX = std::max({maxX, line.p1().x(), line.p2().x()});
        maxY = std::max({maxY, line.p1().y(), line.p2().y()});
        maxZ = std::max({maxZ, line.p1().z(), line.p2().z()});

        minX = std::min({minX, line.p1().x(), line.p2().x()});
        minY = std::min({minY, line.p1().y(), line.p2().y()});
        minZ = std::min({minZ, line.p1().z(), line.p2().z()});
    }

    // Boxed hitbox out of 8 points
    constructFrom2Points(Vec3D(minX, minY, minZ), Vec3D(maxX, maxY, maxZ));
}

void HitBox::generateDetailedFromLineMesh(const LineMesh &lineMesh) {
    // we do not need to add the same points in hit box
    std::set<Vec3D, HitBox::Vec3DLess> points;

    for (const auto& line : lineMesh.lines()) {
        points.insert(Vec3D(line.p1()));
        points.insert(Vec3D(line.p2()));
    }

    _hitBox.reserve(points.size());
    for (const auto& it : points)
        _hitBox.push_back(it);
    _hitBox.shrink_to_fit();
}

HitBox::~HitBox() {
    _hitBox.clear();
}

void HitBox::constructFrom2Points(const Vec3D &from, const Vec3D &to) {
    _hitBox.emplace_back(from.x(), from.y(), from.z());
    _hitBox.emplace_back(from.x(), to.y(), from.z());
    _hitBox.emplace_back(to.x(), from.y(), from.z());
    _hitBox.emplace_back(to.x(), to.y(), from.z());

    _hitBox.emplace_back(from.x(), from.y(), to.z());
    _hitBox.emplace_back(from.x(), to.y(), to.z());
    _hitBox.emplace_back(to.x(), from.y(), to.z());
    _hitBox.emplace_back(to.x(), to.y(), to.z());
}
