//
// Created by Иван Ильин on 24/01/2024.
//

#ifndef INC_3DZAVR_TEST_GROUP_H
#define INC_3DZAVR_TEST_GROUP_H

#include <objects/Object.h>
#include "utils/FileName.h"

class Group : public Object {
private:
    std::map<ObjectTag, std::shared_ptr<Object>> _objects;

    void copyObjectsFromGroup(const Group& group);
public:
    explicit Group(const ObjectTag& tag): Object(tag) {};
    explicit Group(const ObjectTag& tag, const FileName &mesh_file, const Vec3D &scale = Vec3D{1, 1, 1});
    Group(const Group& group);
    Group(const ObjectTag& tag, const Group& group);

    void add(std::shared_ptr<Object> object);
    void add(std::shared_ptr<Group> group);
    void add(const ObjectTag& tag, const FileName &mesh_file, const Vec3D &scale = Vec3D{1, 1, 1});

    bool remove(const ObjectTag &tag);
    std::shared_ptr<Object> object(const ObjectTag &tag);

    std::map<ObjectTag, std::shared_ptr<Object>>::iterator begin() { return _objects.begin(); }
    std::map<ObjectTag, std::shared_ptr<Object>>::iterator end() { return _objects.end(); }

    std::map<ObjectTag, std::shared_ptr<Object>> objects() const { return _objects; }



    [[nodiscard]] IntersectionInformation intersect(const Vec3D &from, const Vec3D &to) const override;
    [[nodiscard]] IntersectionInformation rayCast(const Vec3D &from, const Vec3D &to,
                                                  const std::set<ObjectTag> &skipTags = {}) const;
};


#endif //INC_3DZAVR_TEST_GROUP_H
