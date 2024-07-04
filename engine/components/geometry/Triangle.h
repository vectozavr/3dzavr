#ifndef GEOMETRY_TRIANGLE_H
#define GEOMETRY_TRIANGLE_H

#include "linalg/Vec4D.h"
#include "linalg/Vec3D.h"
#include "linalg/Matrix4x4.h"
#include "Consts.h"


/**
 * @class Triangle
 * @brief Represents a triangle in 3D space.
 *
 * This class encapsulates a triangle defined by three points in 3D space. It provides functionality
 * to compute various geometric properties such as the centroid, normal, and texture coordinates. It also
 * supports operations like intersection with a line segment and transformation by a 4x4 matrix.
 */
class Triangle final {
private:
    /**
     * @struct IntersectionInformation
     * @brief Holds information about an intersection with the triangle.
     *
     * This structure contains details of an intersection point between a line segment and the triangle,
     * including the point of intersection, barycentric coordinates, distance to the triangle, and a flag
     * indicating whether an intersection occurred.
     */
    struct IntersectionInformation final {
        Vec3D pointOfIntersection; ///< The point of intersection.
        Vec3D abg; ///< Barycentric coordinates of the intersection point.
        double distanceToTriangle = std::numeric_limits<double>::infinity(); ///< Distance from the line's start to the intersection point.
        bool intersected = false; ///< Flag indicating whether an intersection occurred.
    };

    std::array<Vec4D, 3> _points; ///< The vertices of the triangle in 3D space.
    /**
     * Texture coordinates are in 3D because we need to work with its 3th component
     * during de-homogenization procedure for correct perspective projection
     * (see Screen::drawTriangle() for example).
     */
    std::array<Vec3D, 3> _textureCoordinates; ///< The texture coordinates associated with the triangle's vertices.

    Vec3D _normal; ///< The normal vector of the triangle.

    /**
     * @brief Calculates the normal vector of the triangle.
     */
    void calculateNormal();
public:
    Triangle() = default;

    Triangle(const Triangle &triangle) = default;

    /**
     * @brief Constructs a Triangle with specified vertices and optional texture coordinates.
     * @param p The vertices of the triangle.
     * @param uv The texture coordinates for the vertices. Defaults to {(0,0,1), (0,0,1), (0,0,1)}.
     */
    Triangle(const std::array<Vec4D, 3>& p, const std::array<Vec3D, 3>& uv = {Vec3D{0, 0, 1},
                                                                              Vec3D{0, 0, 1},
                                                                              Vec3D{0, 0, 1}});

    Triangle &operator=(const Triangle &) = default;

    /**
     * @brief Accesses a vertex of the triangle by index.
     * @param i The index of the vertex to access (0-2).
     * @return The vertex at the specified index.
     */
    [[nodiscard]] inline const Vec4D& operator[](int i) const { return _points[i]; }

    /**
     * @brief Calculates the centroid of the triangle.
     * @return The centroid of the triangle as a Vec3D.
     */
    [[nodiscard]] inline Vec3D centroid() const { return Vec3D(_points[0] + _points[1] + _points[2])/3; }

    /**
     * @brief Retrieves the normal vector of the triangle.
     * @return The normal vector as a Vec3D.
     */
    [[nodiscard]] inline Vec3D norm() const { return _normal; }

    // Operations with Matrix4x4
    /**
     * @brief Transforms the triangle by a given 4x4 matrix.
     *
     * This method applies a transformation matrix to each vertex of the triangle, effectively
     * translating, rotating, and scaling the triangle in 3D space.
     *
     * @param matrix4X4 The transformation matrix to apply.
     * @return A new Triangle instance representing the transformed triangle.
     */
    [[nodiscard]] Triangle operator*(const Matrix4x4 &matrix4X4) const;

    /**
     * @brief Calculates the intersection between a line segment and the triangle.
     *
     * This method determines if a line segment, defined by two points 'from' and 'to', intersects
     * with the triangle. If an intersection occurs, it returns detailed information about the
     * intersection, including the point of intersection, barycentric coordinates, and the distance
     * from the 'from' point to the intersection point.
     *
     * @param from The starting point of the line segment.
     * @param to The ending point of the line segment.
     * @return IntersectionInformation structure containing details of the intersection.
     */
    [[nodiscard]] IntersectionInformation intersect(const Vec3D &from, const Vec3D &to) const;

    /**
     * @brief Accessor for the triangle's vertices.
     *
     * This method provides read-only access to the triangle's vertices, allowing for operations
     * like rendering, collision detection, and more.
     *
     * @return A constant reference to the array of vertices.
     */
    [[nodiscard]] inline const std::array<Vec4D, 3>& points() const {return _points; }

    /**
     * @brief Accessor for the triangle's texture coordinates.
     *
     * This method provides read-only access to the texture coordinates associated with each vertex
     * of the triangle. These coordinates are used for texture mapping during rendering.
     *
     * @return A constant reference to the array of texture coordinates.
     */
    [[nodiscard]] inline const std::array<Vec3D, 3>& textureCoordinates() const { return _textureCoordinates; }

    /**
     * @brief Calculates the distance from a point to the triangle's plane.
     *
     * This method computes the shortest distance from a given point in 3D space to the plane
     * in which the triangle lies. It is useful for collision detection and spatial queries.
     *
     * @param vec The point in 3D space.
     * @return The distance from the point to the triangle's plane.
     */
    [[nodiscard]] double distance(const Vec3D &vec) const { return norm().dot(Vec3D(_points[0]) - vec); }
    

    /**
     * @brief Calculates barycentric coordinates of a point projected onto the triangle's plane.
     *
     * This overload accepts a 2D point, assuming the point lies on the same plane as the triangle.
     * It calculates the barycentric coordinates, which are useful for texture mapping and interpolation.
     *
     * @param point The 2D point to calculate barycentric coordinates for.
     * @return The barycentric coordinates of the point.
     */
    [[nodiscard]] Vec3D abgBarycCoord(const Vec2D& point) const;

    /**
     * @brief Calculates barycentric coordinates of a point projected onto the triangle's plane.
     *
     * This method calculates the barycentric coordinates of a given 3D point with respect to the triangle.
     * Barycentric coordinates are useful for various geometric calculations, including interpolation and
     * determining if a point lies within the triangle. The point is assumed to be on the same plane as the triangle.
     *
     * @param point The 3D point for which to calculate barycentric coordinates.
     * @return The barycentric coordinates of the point as a Vec3D.
     */
    [[nodiscard]] Vec3D abgBarycCoord(const Vec3D& point) const;
};


#endif //GEOMETRY_TRIANGLE_H
