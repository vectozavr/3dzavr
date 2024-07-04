#ifndef OBJECTS_GROUP_H
#define OBJECTS_GROUP_H

#include <objects/Object.h>
#include <components/geometry/TriangleMesh.h>

#include <memory>
#include "utils/FilePath.h"

/**
 * @class Group
 * @brief Represents a collection of objects in the game world.
 *
 * Inherits from Object and provides functionality to manage a group of objects as a single entity.
 * This includes adding, replacing, and removing objects from the group. It can also be used to
 * instantiate groups with specific properties from files or other groups.
 */

class Group : public Object {
private:
    /**
     * Copies objects from another group into this group.
     * @param group The group from which objects are copied.
     */
    void copyObjectsFromGroup(const Group &group);

public:
    /**
     * Constructs a Group with a specific tag.
     * @param tag The tag identifying the group.
     */
    explicit Group(const ObjectTag &tag) : Object(tag) {};

    /**
     * Constructs a Group with a specific tag, loading its content from a mesh file.
     * @param tag The tag identifying the group.
     * @param mesh_file The file path of the mesh to load.
     * @param scale The scale to apply to the loaded mesh (default is 1,1,1).
     */
    explicit Group(const ObjectTag &tag, const FilePath &mesh_file, const Vec3D &scale = Vec3D{1, 1, 1});

    /**
     * Copy constructor.
     * @param group The group to copy from.
     */
    Group(const Group &group);

    /**
     * Constructs a Group with a specific tag, copying objects from another group.
     * @param tag The tag identifying the new group.
     * @param group The group from which objects are copied.
     */
    Group(const ObjectTag &tag, const Group &group);

    /**
     * Adds an object to the group.
     * @param object The object to add.
     */
    void add(std::shared_ptr<Object> object);

    /**
     * Adds an object to the group by value.
     * @param object The object to add.
     */
    void add(const Object &object);

    /**
     * Adds an object to the group, loading it from a mesh file.
     * @param tag The tag identifying the new object.
     * @param mesh_file The file path of the mesh to load.
     * @param scale The scale to apply to the loaded mesh (default is 1,1,1).
     */
    void add(const ObjectTag &tag, const FilePath &mesh_file, const Vec3D &scale = Vec3D{1, 1, 1});

    /**
     * Replaces an existing object in the group with a new one.
     * @param object The new object to replace the existing one.
     */
    void replace(std::shared_ptr<Object> object);

    /**
     * Replaces an existing object in the group with a new one by value.
     * @param object The new object to replace the existing one.
     */

    void replace(const Object &object);

    /**
     * Removes an object with the specified tag from the group.
     * @param tag The tag of the object to remove.
     * @return True if the object was successfully removed, false otherwise.
     */
    bool remove(const ObjectTag &tag);

    /**
     * Clears all objects from the group.
     */
    void clear();

    /**
     * Returns the number of objects currently in the group.
     * @return The number of objects in the group.
     */
    [[nodiscard]] uint16_t size() const { return numberOfAttached(); }

    /**
     * Finds an object with the specified tag in the group.
     * @param tag The tag of the object to find.
     * @return A shared pointer to the found object, or nullptr if not found.
     */
    std::shared_ptr<Object> find(const ObjectTag &tag);

    /**
     * Performs a ray cast from 'from' to 'to' and returns information about any intersection.
     * @param from The starting point of the ray.
     * @param to The ending point of the ray.
     * @param skipTags Tags of objects to skip during the intersection test.
     * @return Information about the intersection, if any.
     */
    [[nodiscard]] TriangleMesh::IntersectionInformation intersect(const Vec3D &from, const Vec3D &to,
                                                                  const std::set<ObjectTag> &skipTags = {}) const;

    /**
     * Creates a copy of the group with a new tag.
     * @param tag The tag for the new group copy.
     * @return A shared pointer to the copied group.
     */
    [[nodiscard]] std::shared_ptr<Object> copy(const ObjectTag &tag) const override {
        return std::make_shared<Group>(tag, *this);
    }

    /**
     * Destructor for the Group class, responsible for cleaning up resources.
     */
    ~Group() override;
};


#endif //OBJECTS_GROUP_H
