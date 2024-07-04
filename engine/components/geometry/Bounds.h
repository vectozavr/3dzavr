//
// Created by Neiro on 28.2.2024.
//

#ifndef GEOMETRY_BOUNDS_H
#define GEOMETRY_BOUNDS_H

#include "linalg/Matrix4x4.h"

/**
 * @struct Bounds
 * @brief Represents the bounds of a 3D object, defined by its center and extents in each direction.
 *
 * This structure is used to define the spatial boundaries of 3D objects, providing a central point
 * and the half-lengths (extents) of the object along each axis. It supports transformation by a 4x4 matrix,
 * allowing for the bounds to be scaled, rotated, and translated.
 */
struct Bounds {
    Vec3D center; ///< The center point of the bounds.
    Vec3D extents; ///< The half-lengths of the bounds along the X, Y, and Z axes.

    /**
     * @brief Transforms the bounds by a given 4x4 matrix.
     *
     * This operation applies a transformation matrix to the bounds, affecting its center and extents.
     * The transformation includes translation, rotation, and scaling, based on the matrix provided.
     * The extents are adjusted to account for rotation and scaling, ensuring the bounds remain accurate.
     *
     * @param matrix4X4 The transformation matrix to apply to the bounds.
     * @return A new Bounds instance representing the transformed bounds.
     */
    [[nodiscard]] inline Bounds operator*(const Matrix4x4 &matrix4X4) const {
        Vec3D newCenter = Vec3D(matrix4X4 * center.makePoint4D()); // Transforms the center point.
        Vec3D left = matrix4X4.x() * extents.x(); // Scales the extents along the X axis.
        Vec3D up = matrix4X4.y() * extents.y(); // Scales the extents along the Y axis.
        Vec3D forward = matrix4X4.z() * extents.z(); // Scales the extents along the Z axis.
        Vec3D newExtents(
                std::abs(left.x()) + std::abs(up.x()) + std::abs(forward.x()),
                std::abs(left.y()) + std::abs(up.y()) + std::abs(forward.y()),
                std::abs(left.z()) + std::abs(up.z()) + std::abs(forward.z())
        ); // Calculates the new extents, taking into account potential rotation.

        return {newCenter, newExtents}; // Returns the transformed bounds.
    }
};

#endif //GEOMETRY_BOUNDS_H
