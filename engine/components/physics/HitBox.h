#ifndef PHYSICS_HITBOX_H
#define PHYSICS_HITBOX_H

#include "components/geometry/TriangleMesh.h"
#include "components/geometry/LineMesh.h"

/**
 * @class HitBox
 * @brief Represents a hitbox for collision detection in a physics engine.
 *
 * This class encapsulates the functionality to create and manage a hitbox, which is a simplified representation
 * of an object's shape used for collision detection. It supports generating hitboxes from both triangle and line meshes,
 * with options for simple or detailed representations.
 */
class HitBox final {
private:
    struct Vec3DLess {
        bool operator()(const Vec3D& lhs, const Vec3D& rhs) const noexcept;
    };

    std::vector<Vec3D> _hitBox;

    /**
     * Generates a simplified hitbox from a TriangleMesh object.
     * This method simplifies the mesh to its bounding box for efficient collision detection.
     *
     * @param triangleMesh The TriangleMesh object to generate the hitbox from.
     */
    void generateSimpleFromTriangleMesh(const TriangleMesh &triangleMesh);

    /**
     * Generates a detailed hitbox from a TriangleMesh object.
     * This method generates a detailed hitbox from the mesh for more accurate collision detection.
     *
     * @param triangleMesh The TriangleMesh object to generate the hitbox from.
     */
    void generateDetailedFromTriangleMesh(const TriangleMesh &triangleMesh);

    /**
     * Generates a simplified hitbox from a LineMesh object.
     * This method simplifies the mesh to its bounding box for efficient collision detection.
     *
     * @param lineMesh The LineMesh object to generate the hitbox from.
     */
    void generateSimpleFromLineMesh(const LineMesh &lineMesh);

    /**
     * Generates a detailed hitbox from a LineMesh object.
     * This method generates a detailed hitbox from the mesh for more accurate collision detection.
     *
     * @param lineMesh The LineMesh object to generate the hitbox from.
     */
    void generateDetailedFromLineMesh(const LineMesh &lineMesh);

    /**
     * Constructs a hitbox from two 3D points in space.
     * This method constructs a hitbox from two points, representing the minimum and maximum points of the hitbox.
     *
     * @param from The minimum point of the hitbox.
     * @param to The maximum point of the hitbox.
     */
    void constructFrom2Points(const Vec3D& from, const Vec3D& to);
public:
    HitBox() = default;
    HitBox(const HitBox &hitBox) = default;

    /**
     * Constructor for creating a HitBox from a TriangleMesh.
     * Allows for the creation of either a simplified or detailed hitbox based on the TriangleMesh.
     *
     * @param triangleMesh The TriangleMesh object from which to generate the hitbox.
     * @param useSimpleBox If true, generates a simplified hitbox; otherwise, generates a detailed hitbox.
     */
    explicit HitBox(const TriangleMesh &triangleMesh, bool useSimpleBox = true);

    /**
     * Constructor for creating a HitBox from a LineMesh.
     * Allows for the creation of either a simplified or detailed hitbox based on the LineMesh.
     *
     * @param lineMesh The LineMesh object from which to generate the hitbox.
     * @param useSimpleBox If true, generates a simplified hitbox; otherwise, generates a detailed hitbox.
     */
    explicit HitBox(const LineMesh &lineMesh, bool useSimpleBox = true);

    /**
     * Returns the number of vertices in the hitbox.
     * This can be used to determine the complexity or size of the hitbox.
     *
     * @return The number of vertices in the hitbox.
     */
    [[nodiscard]] size_t size() const { return _hitBox.size(); }

    /**
     * Returns whether the hitbox is empty.
     * An empty hitbox has no vertices and is not valid for collision detection.
     *
     * @return True if the hitbox is empty; false otherwise.
     */
    [[nodiscard]] bool empty() const { return _hitBox.empty(); }

    /**
     * Provides an iterator to the beginning of the hitbox vertices.
     * This can be used for iterating over the vertices of the hitbox.
     *
     * @return An iterator pointing to the first vertex in the hitbox.
     */
    [[nodiscard]] std::vector<Vec3D>::iterator begin() { return _hitBox.begin(); }

    /**
     * Provides an iterator to the end of the hitbox vertices.
     * This can be used for iterating over the vertices of the hitbox, in conjunction with begin().
     *
     * @return An iterator pointing past the last vertex in the hitbox.
     */
    [[nodiscard]] std::vector<Vec3D>::iterator end() { return _hitBox.end(); }

    /**
     * Destructor for the HitBox class.
     * Cleans up any resources used by the HitBox, ensuring proper memory management.
     */
    ~HitBox();
};


#endif //PHYSICS_HITBOX_H
