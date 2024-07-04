#ifndef PHYSICS_GJK_H
#define PHYSICS_GJK_H

#include <components/physics/Simplex.h>
#include <linalg/Vec3D.h>

class RigidObject;

namespace GJK {

    /**
     * @struct FaceNormal
     * @brief Represents the normal vector and distance of a face used in the GJK algorithm.
     *
     * This structure is utilized within the Gilbert-Johnson-Keerthi (GJK) collision detection algorithm
     * to represent the normal vector of a face and its distance from the origin. These attributes are
     * essential in determining the closest point on a simplex to the origin, which aids in collision detection.
     *
     * @var normal The normal vector of the face.
     * @var distance The distance of the face from the origin.
     */
    struct FaceNormal final {
        const Vec3D normal;
        const double distance;
    };

    /**
     * @struct NextSimplex
     * @brief Encapsulates the result of evolving a simplex in the GJK algorithm.
     *
     * This structure is used to represent the outcome of processing a simplex through one iteration
     * of the GJK algorithm. It includes the new simplex to be used in the next iteration, the new search
     * direction, and a flag indicating whether the algorithm should finish searching.
     *
     * @var newSimplex The simplex for the next iteration.
     * @var newDirection The direction in which to search for the next point to add to the simplex.
     * @var finishSearching Flag indicating whether the GJK algorithm has determined collision or not.
     */
    struct NextSimplex final {
        const Simplex newSimplex;
        const Vec3D newDirection;
        const bool finishSearching;
    };

    /**
     * @brief Computes the support point of the Minkowski difference of two objects in a given direction.
     *
     * This function calculates the support point of the Minkowski difference of two objects in the specified
     * direction. The support point is used to determine the next point to add to the simplex in the GJK algorithm.
     *
     * @param obj1 The first object in the collision detection.
     * @param obj2 The second object in the collision detection.
     * @param direction The direction in which to compute the support point.
     * @return The support point of the Minkowski difference in the given direction.
     */
    [[nodiscard]] SupportPoint support(const std::shared_ptr<RigidObject>& obj1,
                                      const std::shared_ptr<RigidObject>& obj2,
                                      const Vec3D &direction);

    /**
     * Determines the next simplex configuration and search direction.
     *
     * This function evaluates the current simplex and decides how to evolve it for the next iteration
     * of the GJK algorithm. It determines the new simplex, the next search direction, and whether the
     * algorithm should terminate.
     *
     * @param points The current simplex.
     * @return The next simplex configuration and search direction.
     */
    static NextSimplex nextSimplex(const Simplex &points);

    /**
     * Determines the next simplex configuration and search direction for a line.
     *
     * This function processes a line simplex and determines the next simplex configuration and search
     * direction for the next iteration of the GJK algorithm. It is used to evolve the simplex in the
     * GJK algorithm when the current simplex is a line.
     *
     * @param points The current line simplex.
     * @return The next simplex configuration and search direction.
     */
    static NextSimplex lineCase(const Simplex &points);

    /**
     * Determines the next simplex configuration and search direction for a triangle.
     *
     * This function processes a triangle simplex and determines the next simplex configuration and search
     * direction for the next iteration of the GJK algorithm. It is used to evolve the simplex in the
     * GJK algorithm when the current simplex is a triangle.
     *
     * @param points The current triangle simplex.
     * @return The next simplex configuration and search direction.
     */
    static NextSimplex triangleCase(const Simplex &points);

    /**
     * Determines the next simplex configuration and search direction for a tetrahedron.
     *
     * This function processes a tetrahedron simplex and determines the next simplex configuration and search
     * direction for the next iteration of the GJK algorithm. It is used to evolve the simplex in the
     * GJK algorithm when the current simplex is a tetrahedron.
     *
     * @param points The current tetrahedron simplex.
     * @return The next simplex configuration and search direction.
     */
    static NextSimplex tetrahedronCase(const Simplex &points);

    /**
     * Checks for collision between two objects using the GJK algorithm.
     *
     * This function performs collision detection between two objects using the Gilbert-Johnson-Keerthi (GJK)
     * algorithm. It constructs a simplex from the support points of the two objects and iteratively refines
     * the simplex to determine if the objects are colliding.
     *
     * @param obj1 The first object in the collision detection.
     * @param obj2 The second object in the collision detection.
     * @return A pair containing a flag indicating collision and the simplex representing the collision.
     */
    [[nodiscard]] std::pair<bool, Simplex> checkGJKCollision(const std::shared_ptr<RigidObject>& obj1,
                                                             const std::shared_ptr<RigidObject>& obj2);
};


#endif //PHYSICS_GJK_H
