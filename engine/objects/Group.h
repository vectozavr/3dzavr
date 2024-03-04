#ifndef OBJECTS_GROUP_H
#define OBJECTS_GROUP_H

#include <objects/Object.h>

#include <memory>
#include "utils/FilePath.h"

class Group : public Object {
private:
    std::map<ObjectTag, std::shared_ptr<Object>> _objects;

    void copyObjectsFromGroup(const Group& group);
public:
    explicit Group(const ObjectTag& tag): Object(tag) {};
    explicit Group(const ObjectTag& tag, const FilePath &mesh_file, const Vec3D &scale = Vec3D{1, 1, 1});

    Group(const Group& group);
    Group(const ObjectTag& tag, const Group& group);

    void add(std::shared_ptr<Object> object);
    void add(const ObjectTag& tag, const FilePath &mesh_file, const Vec3D &scale = Vec3D{1, 1, 1});

    bool remove(const ObjectTag &tag);
    void clear();
    [[nodiscard]] uint16_t size() const {return _objects.size(); }
    std::shared_ptr<Object> object(const ObjectTag &tag);

    std::map<ObjectTag, std::shared_ptr<Object>>::iterator begin() { return _objects.begin(); }
    std::map<ObjectTag, std::shared_ptr<Object>>::iterator end() { return _objects.end(); }

    std::map<ObjectTag, std::shared_ptr<Object>>::const_iterator begin() const { return _objects.begin(); }
    std::map<ObjectTag, std::shared_ptr<Object>>::const_iterator end() const { return _objects.end(); }

    [[nodiscard]] IntersectionInformation intersect(const Vec3D &from, const Vec3D &to) override;
    [[nodiscard]] IntersectionInformation rayCast(const Vec3D &from, const Vec3D &to,
                                                  const std::set<ObjectTag> &skipTags = {}) const;

    std::shared_ptr<Object> copy(const ObjectTag& tag) const override {
        return std::make_shared<Group>(tag, *this);
    }

    ~Group() override;
};


#endif //OBJECTS_GROUP_H
