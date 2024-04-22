#ifndef OBJECTS_GROUP_H
#define OBJECTS_GROUP_H

#include <objects/Object.h>
#include <components/geometry/TriangleMesh.h>

#include <memory>
#include "utils/FilePath.h"

class Group : public Object {
private:
    void copyObjectsFromGroup(const Group& group);
public:
    explicit Group(const ObjectTag& tag): Object(tag) {};
    explicit Group(const ObjectTag& tag, const FilePath &mesh_file, const Vec3D &scale = Vec3D{1, 1, 1});

    Group(const Group& group);
    Group(const ObjectTag& tag, const Group& group);

    void add(std::shared_ptr<Object> object);
    void add(const Object& object);
    void add(const ObjectTag& tag, const FilePath &mesh_file, const Vec3D &scale = Vec3D{1, 1, 1});

    void replace(std::shared_ptr<Object> object);
    void replace(const Object& object);

    bool remove(const ObjectTag &tag);
    void clear();
    [[nodiscard]] uint16_t size() const { return numberOfAttached(); }
    std::shared_ptr<Object> find(const ObjectTag &tag);

    [[nodiscard]] TriangleMesh::IntersectionInformation intersect(const Vec3D &from, const Vec3D &to,
                                                                  const std::set<ObjectTag> &skipTags = {}) const;

    [[nodiscard]] std::shared_ptr<Object> copy(const ObjectTag& tag) const override {
        return std::make_shared<Group>(tag, *this);
    }

    ~Group() override;
};


#endif //OBJECTS_GROUP_H
