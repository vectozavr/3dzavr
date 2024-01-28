//
// Created by Иван Ильин on 24/01/2024.
//

#include "Group.h"
#include "utils/Log.h"
#include "utils/ResourceManager.h"

void Group::copyObjectsFromGroup(const Group& group) {
    for (auto& [tag, obj] : group._objects) {
        std::shared_ptr<Group> grObj = std::dynamic_pointer_cast<Group>(obj);
        if(grObj) {
            // We work with Group
            auto copedGroup = std::make_shared<Group>(*grObj);
            add(copedGroup);
        } else {
            // We work with the object (e.g. Mesh)
            // TODO: here we cast obj to the type Object. We need to fix it somehow..
            auto copedObject = std::make_shared<Object>(*obj);
            add(copedObject);
        }
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
    _objects.emplace(object->name(), object);
    attach(object);
    Log::log("Group::add(): inserted object '" + object->name().str() + "' in the group '" + name().str() + "'");
}

void Group::add(std::shared_ptr<Group> group) {
    _objects.emplace(group->name(), group);
    Log::log("Group::add(): inserted group '" + group->name().str() + "'");
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

    for(auto& obj : _objects) {

        std::shared_ptr<Group> grObj = std::dynamic_pointer_cast<Group>(obj.second);
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

Object::IntersectionInformation Group::intersect(const Vec3D &from, const Vec3D &to) const {
    std::unique_ptr<IntersectionInformation> minIntersection;

    for(auto& obj: _objects) {
        auto intersection = obj.second->intersect(from, to);
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

    for(const auto& obj: _objects) {

        if(skipTags.contains(obj.first)) {
            continue;
        }

        std::shared_ptr<Group> grObj = std::dynamic_pointer_cast<Group>(obj.second);

        std::shared_ptr<IntersectionInformation> intersection;
        if(grObj) {
            // We work with Group
            intersection = std::make_shared<IntersectionInformation>(grObj->rayCast(from, to, skipTags));
        } else {
            // We work with the object (e.g. Mesh)
            intersection = std::make_shared<IntersectionInformation>(obj.second->intersect(from, to));
            intersection->obj = obj.second;
        }

        if(intersection->distanceToObject < minIntersection->distanceToObject) {
            minIntersection = intersection;
        }
    }

    return *minIntersection;
}

Group::~Group() {
    _objects.clear();
}
