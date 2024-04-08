//
// Created by Neiro on 28.2.2024.
//

#ifndef GEOMETRY_BOUNDS_H
#define GEOMETRY_BOUNDS_H

#include "linalg/Matrix4x4.h"

struct Bounds {
    Vec3D center;
    Vec3D extents;

    [[nodiscard]] inline Bounds operator*(const Matrix4x4 &matrix4X4) const {
        Vec3D newCenter = Vec3D(matrix4X4 * center.makePoint4D());
        Vec3D left = matrix4X4.x() * extents.x();
        Vec3D up = matrix4X4.y() * extents.y();
        Vec3D forward = matrix4X4.z() * extents.z();
        Vec3D newExtents(
                std::abs(left.x()) + std::abs(up.x()) + std::abs(forward.x()),
                std::abs(left.y()) + std::abs(up.y()) + std::abs(forward.y()),
                std::abs(left.z()) + std::abs(up.z()) + std::abs(forward.z())
        );

        return {newCenter, newExtents};
    }
};

#endif //GEOMETRY_BOUNDS_H
