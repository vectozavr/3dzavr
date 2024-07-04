#ifndef ENGINE_WORLD_H
#define ENGINE_WORLD_H

#include <map>

#include <objects/Camera.h>
#include <objects/Group.h>
#include <io/Screen.h>
#include <objects/Object.h>
#include <components/physics/RigidObject.h>
#include <components/lighting/DirectionalLight.h>


/**
 * @class World
 * @brief Represents the game world, inheriting from Group.
 *
 * This class is responsible for managing the game world, including objects within it,
 * handling collisions, updating the state of the world, and loading objects. It extends
 * the Group class.
 */
class World final : public Group {
private:
    /**
     * Checks for collisions within a specified object or group.
     * @param whereToCheck The object or group to check for collisions.
     */
    void checkCollision(const std::shared_ptr<Object>& whereToCheck);

    /**
     * Checks for collisions between two specified objects or groups.
     * @param whereToCheck The first object or group to check for collisions.
     * @param whatToCheck The second object or group to check for collisions.
     */
    void checkCollision(const std::shared_ptr<Object>& whereToCheck, const std::shared_ptr<Object>& whatToCheck);

    /**
     * Static method to check collision between two objects.
     * @param obj1 The first object.
     * @param obj2 The second object.
     */
    static void checkCollisionBetweenTwo(const std::shared_ptr<Object>& obj1, const std::shared_ptr<Object>& obj2);

public:
    /**
     * Constructor for the World class.
     * @param sceneName The name of the scene or world.
     */
    explicit World(const ObjectTag& sceneName) : Group(sceneName) {};

    /**
     * Updates the state of the world, including all objects within it.
     */
    void update();

    /**
     * Loads an object into the world from a mesh file.
     * @param tag The tag to identify the object.
     * @param meshFile The file path of the mesh to load.
     * @param scale The scale to apply to the object (default is 1,1,1).
     * @return A shared pointer to the loaded group of objects.
     */
    std::shared_ptr<Group> loadObject(const ObjectTag &tag,
                                      const FilePath &meshFile,
                                      const Vec3D &scale = Vec3D{1, 1, 1});

    /**
     * Performs a ray cast in the world to detect intersections with objects.
     * @param from The starting point of the ray.
     * @param to The ending point of the ray.
     * @param skipTags A set of object tags to skip during ray casting.
     * @return Information about the intersection, if any.
     */
    TriangleMesh::IntersectionInformation rayCast(const Vec3D &from, const Vec3D &to, const std::set<ObjectTag> &skipTags = {});
};


#endif //INC_3DZAVR_WORLD_H
