#ifndef PHYSICS_EPA_H
#define PHYSICS_EPA_H

#include <components/physics/GJK.h>
#include <components/geometry/Plane.h>

namespace EPA {

    /**
     * @struct PlanePoints
     * @brief Represents a collection of points and normals defining a plane, along with utility functions for geometric calculations.
     *
     * This structure is used to represent a set of points and their corresponding normals on a plane. It includes methods
     * for checking if a point is inside the plane, finding intersections with a line, computing normals, and computing the origin.
     */
    struct PlanePoints final {
        std::vector<Vec3D> points; ///< Points on the plane.
        std::vector<Vec3D> normals; ///< Normals at each point on the plane.
        Vec3D origin; ///< The origin point of the plane.

        [[nodiscard]] bool isPointInside(const Vec3D& point) const; ///< Checks if a given point is inside the plane.
        [[nodiscard]] std::vector<Vec3D> intersectionsWithLine(const Vec3D& a, const Vec3D& b) const; ///< Finds intersections of the plane with a line.

        void computeNormals(); ///< Computes the normals for the plane points.
        void computeOrigin(); ///< Computes the origin of the plane.
    };

    /**
     * @struct CollisionPoint
     * @brief Represents a collision point including the collision plane, normal, depth, and edges of the collision.
     *
     * This structure encapsulates all relevant information about a collision point, including the plane on which the collision
     * occurred, the normal at the collision point, the depth of penetration, and the edges defining the collision.
     */
    struct CollisionPoint final {
        //const Vec3D point; // Commented out, potentially for future use.
        PlanePoints collisionPlane; ///< The plane on which the collision occurred.
        const Vec3D normal; ///< The normal at the collision point.
        const double depth; ///< The depth of penetration at the collision point.
        const SupportPoint edge1; ///< The first edge of the collision.
        const SupportPoint edge2; ///< The second edge of the collision.
        const SupportPoint edge3; ///< The third edge of the collision.
        const std::vector<SupportPoint> polytope; ///< The polytope formed by the collision points.
    };

    /**
     * Calculates face normals for a given polytope and set of faces.
     *
     * @param polytope A vector of SupportPoints defining the polytope.
     * @param faces A vector of indices representing the faces of the polytope.
     * @return A pair containing a vector of FaceNormals and the size of the vector.
     */
    std::pair<std::vector<GJK::FaceNormal>, size_t>
    getFaceNormals(const std::vector<SupportPoint> &polytope, const std::vector<size_t> &faces);

    /**
     * Adds an edge to a vector of edges if it is unique.
     *
     * @param edges A vector of edges to which the new edge will be added.
     * @param faces A vector of indices representing the faces of the polytope.
     * @param a The index of the first point of the edge.
     * @param b The index of the second point of the edge.
     * @return A vector of edges with the new edge added if it is unique.
     */
    std::vector<std::pair<size_t, size_t>>
    addIfUniqueEdge(const std::vector<std::pair<size_t, size_t>> &edges, const std::vector<size_t> &faces, size_t a,
                    size_t b);

    /**
     * @brief Performs the Expanding Polytope Algorithm (EPA) to find the penetration depth and collision normal between two colliding objects.
     *
     * This function uses the EPA algorithm on the given simplex from the GJK collision detection phase to find the penetration depth and
     * the collision normal of two colliding rigid objects. It returns a CollisionPoint structure containing detailed information about the
     * collision, including the collision plane, normal, depth, and edges of the collision.
     *
     * @param obj1 A shared pointer to the first rigid object involved in the collision.
     * @param obj2 A shared pointer to the second rigid object involved in the collision.
     * @param simplex The simplex resulting from the GJK collision detection phase, representing the initial penetration.
     * @return A CollisionPoint structure containing detailed information about the collision.
     */
    [[nodiscard]] CollisionPoint EPA(std::shared_ptr<RigidObject> obj1,
                                     std::shared_ptr<RigidObject> obj2,
                                     const Simplex &simplex);

    /**
     * @brief Calculates a single collision point given three support points and a normal.
     *
     * This function is used to calculate a precise collision point from three support points defining a triangle on the
     * Minkowski difference and a collision normal. It may be used in further collision response calculations.
     *
     * @note This function cannot compute all collision points in a collision, but only a single point.
     *
     * @param edge1 The first support point of the collision edge.
     * @param edge2 The second support point of the collision edge.
     * @param edge3 The third support point of the collision edge.
     * @param normal The normal vector at the collision point.
     * @return The calculated collision point as a Vec3D.
     */
    [[maybe_unused]] Vec3D calculateSingleCollisionPoint(const SupportPoint& edge1,
                                                                const SupportPoint& edge2,
                                                                const SupportPoint& edge3,
                                                                const Vec3D& normal);

    // Helping functions for calculateCollisionPoints():
    PlanePoints findSortedPointsFromPlane(std::shared_ptr<RigidObject> obj, const Plane& plane);
    PlanePoints findPlanePointsIntersection(const PlanePoints& points1, const PlanePoints& points2);

    /**
     * @brief Calculates the collision points between two rigid objects given a collision normal.
     *
     * This function calculates the collision points between two rigid objects given a collision normal. It uses the
     * GJK algorithm to find the initial simplex and then uses the EPA algorithm to find the collision points. It returns
     * a PlanePoints structure containing the points and normals defining the collision plane.
     *
     * @param obj1 A shared pointer to the first rigid object involved in the collision.
     * @param obj2 A shared pointer to the second rigid object involved in the collision.
     * @param normal The collision normal between the two objects.
     * @return A PlanePoints structure containing the points and normals defining the collision plane.
     */
    PlanePoints calculateCollisionPoints(std::shared_ptr<RigidObject> obj1,
                                                       std::shared_ptr<RigidObject> obj2,
                                                       const Vec3D& normal);

    /**
     * @brief Solves the collision between two rigid objects using the provided collision normal and penetration depth.
     *
     * This function is responsible for resolving collisions between two rigid objects. It uses the collision normal and
     * penetration depth obtained from the collision detection phase (e.g., using GJK and EPA algorithms) to apply the
     * appropriate response to the objects involved. This may include adjusting their positions to resolve interpenetration
     * and applying forces or impulses to simulate physical reactions to the collision.
     *
     * @param obj1 A shared pointer to the first rigid object involved in the collision.
     * @param obj2 A shared pointer to the second rigid object involved in the collision.
     * @param normal The collision normal vector, pointing from obj1 to obj2.
     * @param depth The penetration depth of the collision, indicating how much the objects overlap.
     */
    void solveCollision(std::shared_ptr<RigidObject> obj1, std::shared_ptr<RigidObject> obj2, const Vec3D& normal, double depth);
};


#endif //PHYSICS_EPA_H
