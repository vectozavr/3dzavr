#ifndef GEOMETRY_PLANE_H
#define GEOMETRY_PLANE_H

#include <vector>

#include "linalg/Vec4D.h"
#include "objects/Object.h"
#include "utils/stack_vector.h"
#include "Triangle.h"
#include "Line.h"


/**
 * @class Plane
 * @brief Represents a geometric plane in 3D space.
 *
 * This class models a plane in 3D space, defined either by a normal vector and a point on the plane,
 * a normal vector and an offset from the origin, or by a triangle lying on the plane. It provides
 * functionality to calculate the distance from a point to the plane, find the intersection between
 * a line segment and the plane, and clip lines or line segments with the plane.
 */
class Plane final {
private:
    /**
     * @struct IntersectionInformation
     * @brief Holds information about an intersection with the plane.
     *
     * This structure contains details of an intersection between a line segment and the plane,
     * including the point of intersection, the distance to the object, a parameter indicating
     * the relative position of the intersection point along the line segment, and a flag indicating
     * whether an intersection occurred.
     */
    struct IntersectionInformation final {
        Vec3D pointOfIntersection; ///< The point at which the line intersects the plane.
        double distanceToObject = std::numeric_limits<double>::infinity(); ///< The distance from the start of the line to the intersection point.
        double k = 0; ///< The parameter (0 <= k <= 1) indicating the relative position of the intersection along the line segment.
        bool intersected = false; ///< Flag indicating whether an intersection occurred.
    };
public:
    Vec3D normal; ///< The normal vector of the plane, defining its orientation in 3D space.
    Vec3D point; ///< A point on the plane, used with the normal vector to define the plane's position.
    double offset; ///< The offset from the origin to the plane along the normal vector.

    /**
     * @brief Default constructor. Initializes a plane with default values.
     */
    Plane();

    /**
     * @brief Copy constructor. Creates a new Plane as a copy of an existing one.
     * @param plane The Plane to copy from.
     */
    Plane(const Plane &plane) = default;

    /**
     * @brief Constructor that defines a plane based on a triangle lying on it.
     * @param tri A Triangle object lying on the plane.
     */
    explicit Plane(const Triangle &tri);

    /**
     * @brief Constructor that defines a plane with a normal vector and a point on the plane.
     * @param normal The normal vector of the plane.
     * @param point A point on the plane.
     */
    Plane(const Vec3D &normal, const Vec3D &point);

    /**
     * @brief Constructor that defines a plane with a normal vector and an offset from the origin.
     * @param normal The normal vector of the plane.
     * @param offset The offset from the origin to the plane along the normal vector.
     */
    Plane(const Vec3D &normal, double offset);

    /**
     * @brief Copy assignment operator.
     * @param plane The Plane to copy from.
     * @return Reference to the updated Plane object.
     */
    Plane& operator=(const Plane &plane) = default;

    /**
     * @brief Calculates the shortest distance from a point to the plane.
     * @param point3D The point from which the distance is to be calculated.
     * @return The shortest distance from the point to the plane.
     */
    [[nodiscard]] double distance(const Vec3D &point3D) const;

    /**
     * @brief Calculates the intersection information of a line segment with the plane.
     * @param from The starting point of the line segment.
     * @param to The ending point of the line segment.
     * @return IntersectionInformation containing details of the intersection.
     */
    [[nodiscard]] IntersectionInformation intersect(const Vec3D &from, const Vec3D &to) const;

    /**
     * @brief Clips a set of line segments with the plane, outputting the segments that lie on the visible side of the plane.
     * @param input A vector of pairs of Vec3D, representing the line segments to be clipped.
     * @param output A vector of pairs of Vec3D, where the resulting line segments will be stored.
     */
    void clip(std::vector<std::pair<Vec3D, Vec3D>>& input, std::vector<std::pair<Vec3D, Vec3D>>& output) const;

    /**
     * @brief Clips a line with the plane, determining if the line is fully outside the plane.
     *
     * This method calculates the intersection of a given line with the plane and determines whether
     * the line lies entirely on the visible side of the plane, partially intersects, or is fully outside.
     * If the line intersects with the plane, a clipped line segment that lies within the plane boundaries
     * is returned. The isFullyOutside parameter is set to true if the line is fully outside the plane.
     *
     * @param line The line to be clipped against the plane.
     * @param isFullyOutside A reference to a boolean that will be set to true if the line is fully outside the plane, false otherwise.
     * @return A Line object representing the portion of the line that lies within the plane boundaries.
     */
    [[nodiscard]] Line clip(const Line& line, bool& isFullyOutside) const;
};


#endif //GEOMETRY_PLANE_H
