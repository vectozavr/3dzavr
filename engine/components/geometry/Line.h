#ifndef GEOMETRY_LINE_H
#define GEOMETRY_LINE_H


#include "linalg/Vec4D.h"
#include "linalg/Vec3D.h"
#include "linalg/Vec2D.h"
#include "linalg/Matrix4x4.h"
#include "Consts.h"


/**
 * @class Line
 * @brief Represents a line in 3D space defined by two points.
 *
 * This class encapsulates a geometric line defined by two points in 3D space, providing
 * functionality to compute the line's center, perform transformations, and calculate distances.
 */
class Line final {
private:
    Vec4D _p1 = Vec4D(0, 0, 0, 1); ///< First point of the line in homogeneous coordinates.
    Vec4D _p2 = Vec4D(0, 0, 0, 1); ///< Second point of the line in homogeneous coordinates.

public:
    /**
     * @brief Default constructor. Initializes the line with default points.
     */
    Line() = default;

    /**
     * @brief Copy constructor. Creates a new Line as a copy of an existing one.
     * @param line The Line to copy from.
     */
    Line(const Line &line) = default;

    /**
     * @brief Parameterized constructor. Initializes the line with two points.
     * @param p1 The first point of the line.
     * @param p2 The second point of the line.
     */
    Line(const Vec4D& p1, const Vec4D& p2) : _p1(p1), _p2(p2) {};

    /**
     * @brief Copy assignment operator.
     * @param Line to copy from.
     * @return Reference to the updated Line object.
     */
    Line &operator=(const Line &) = default;

    /**
     * @brief Calculates the center of the line.
     * @return The center point of the line as a Vec3D.
     */
    [[nodiscard]] inline Vec3D center() const { return Vec3D(_p1 + _p2)/3; }

    /**
     * @brief Transforms the line by a given matrix.
     * @param matrix4X4 The transformation matrix.
     * @return A new Line object representing the transformed line.
     */
    [[nodiscard]] inline Line operator*(const Matrix4x4 &matrix4X4) const { return {matrix4X4*_p1, matrix4X4*_p2}; };

    /**
     * @brief Calculates the distance from the line's center to a given point.
     * @param vec The point to calculate the distance to.
     * @return The distance as a double.
     */
    [[nodiscard]] inline double distance(const Vec3D &vec) const { return (center() - vec).abs(); }

    /**
     * @brief Getter for the first point of the line.
     * @return The first point of the line as a Vec4D.
     */
    [[nodiscard]] inline Vec4D p1() const { return _p1; }

    /**
     * @brief Getter for the second point of the line.
     * @return The second point of the line as a Vec4D.
     */
    [[nodiscard]] inline Vec4D p2() const { return _p2; }
};

#endif //GEOMETRY_LINE_H
