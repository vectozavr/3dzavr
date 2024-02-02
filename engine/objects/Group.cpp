//
// Created by Иван Ильин on 24/01/2024.
//

#include "Group.h"
#include "utils/Log.h"
#include "utils/ResourceManager.h"

void Group::copyObjectsFromGroup(const Group& group) {
    for (const auto& [tag, obj] : group._objects) {
        add(obj->copy(tag));
    }
}

Group::Group(const Group& group) : Object(group) {
    copyObjectsFromGroup(group);
}

Group::Group(const ObjectTag& tag, const Group& group) : Object(tag, group) {
    copyObjectsFromGroup(group);
}

Group::Group(const ObjectTag& tag, const FilePath &mesh_file, const Vec3D &scale) : Object(tag) {
    auto objects = ResourceManager::loadObject(tag, mesh_file);
    copyObjectsFromGroup(*objects);
}

void Group::add(std::shared_ptr<Object> object) {
    if(!_objects.contains(object->name())) {
        _objects.emplace(object->name(), object);
        attach(object);
        Log::log("Group::add(): inserted object '" + object->name().str() + "' in the group '" + name().str() + "'");
    } else {
        throw std::invalid_argument{"Group::add(): You cannot inserted 2 objects with the same name tag"};
    }
}

void Group::add(const ObjectTag& tag, const FilePath &mesh_file, const Vec3D &scale) {
    auto objects = ResourceManager::loadObject(tag, mesh_file);
    objects->scale(scale);
    add(objects);
}

bool Group::remove(const ObjectTag &tag) {
    if (_objects.erase(tag) > 0) {
        unattach(tag);
        Log::log("Group::remove(): removed '" + tag.str() + "' from the group '" + name().str() + "'");
        return true;
    } else {
        for(auto& obj : _objects) {
            std::shared_ptr<Group> grObj = std::dynamic_pointer_cast<Group>(obj.second);
            if(grObj) {
                if (grObj->remove(tag)) {
                    return true;
                }
            }
        }
    }

    Log::log("Group::remove(): cannot remove '" + tag.str() + "' from the group '" + name().str() + "': there are no such object");
    return false;
}

std::shared_ptr<Object> Group::object(const ObjectTag &tag) {
    if (_objects.count(tag) != 0) {
        return _objects.find(tag)->second;
    }

    for(const auto& [name, obj] : _objects) {

        std::shared_ptr<Group> grObj = std::dynamic_pointer_cast<Group>(obj);
        if(grObj) {
            auto res = grObj->object(tag);
            if (res) {
                return res;
            }
        }
    }

    Log::log("Group::object(): no object with tag '" + tag.str() + "' in the group '" + name().str() + "'");
    return nullptr;
}

Object::IntersectionInformation Group::intersect(const Vec3D &from, const Vec3D &to) {
    std::unique_ptr<IntersectionInformation> minIntersection;

    for(const auto& [name, obj]: _objects) {
        auto intersection = obj->intersect(from, to);
        if(intersection.distanceToObject < minIntersection->distanceToObject) {
            minIntersection = std::make_unique<IntersectionInformation>(intersection);
        }
    }

    return *minIntersection;
}

Object::IntersectionInformation Group::rayCast(const Vec3D &from, const Vec3D &to,
                                               const std::set<ObjectTag> &skipTags) const {
    std::shared_ptr<IntersectionInformation> minIntersection = std::make_shared<IntersectionInformation>();

    if(skipTags.contains(name())) {
        return *minIntersection;
    }

    for(const auto& [tag, obj]: _objects) {

        if(skipTags.contains(tag)) {
            continue;
        }

        std::shared_ptr<Group> grObj = std::dynamic_pointer_cast<Group>(obj);

        std::shared_ptr<IntersectionInformation> intersection;
        if(grObj) {
            intersection = std::make_shared<IntersectionInformation>(grObj->rayCast(from, to, skipTags));
        } else {
            intersection = std::make_shared<IntersectionInformation>(obj->intersect(from, to));
        }

        if(intersection->distanceToObject < minIntersection->distanceToObject && intersection->k > 0) {
            minIntersection = intersection;
        }
    }

    return *minIntersection;
}

void Group::clear() {
    _objects.clear();
}

Group::~Group() {
    clear();
}

