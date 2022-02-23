//
// Created by Иван Ильин on 04.11.2021.
//

#include <set>
#include <cmath>

#include "HitBox.h"
#include "../Consts.h"

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

HitBox::HitBox(const Mesh& mesh, bool useSimpleBox) {
    if (useSimpleBox) {
        generateSimple(mesh);
    } else {
        generateDetailed(mesh);
    }
}

void HitBox::generateSimple(const Mesh &mesh) {
    double maxX = -std::numeric_limits<double>::max();
    double maxY = -std::numeric_limits<double>::max();
    double maxZ = -std::numeric_limits<double>::max();

    double minX = std::numeric_limits<double>::max();
    double minY = std::numeric_limits<double>::max();
    double minZ = std::numeric_limits<double>::max();

    for(const auto& t : mesh.triangles()) {
        for(int i = 0; i < 3; i++) {
            auto point = Vec3D(t[i]);
            if(point.x() > maxX) {
                maxX = point.x();
            }
            if(point.y() > maxY) {
                maxY = point.y();
            }
            if(point.z() > maxZ) {
                maxZ = point.z();
            }

            if(point.x() < minX) {
                minX = point.x();
            }
            if(point.y() < minY) {
                minY = point.y();
            }
            if(point.z() < minZ) {
                minZ = point.z();
            }
        }
    }

    _hitBox.emplace_back(minX, minY, minZ);
    _hitBox.emplace_back(minX, maxY, minZ);
    _hitBox.emplace_back(maxX, minY, minZ);
    _hitBox.emplace_back(maxX, maxY, minZ);

    _hitBox.emplace_back(minX, minY, maxZ);
    _hitBox.emplace_back(minX, maxY, maxZ);
    _hitBox.emplace_back(maxX, minY, maxZ);
    _hitBox.emplace_back(maxX, maxY, maxZ);
}

void HitBox::generateDetailed(const Mesh &mesh) {
    // we dont need to add the same points in hit box
    std::set<Vec3D, HitBox::Vec3DLess> points;

    for (const auto& t : mesh.triangles())
        for (int i = 0; i < 3; i++)
            points.insert(Vec3D(t[i]));

    _hitBox.reserve(points.size());
    for (const auto& it : points)
        _hitBox.push_back(it);
    _hitBox.shrink_to_fit();
}
