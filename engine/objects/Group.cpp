#include "Group.h"
#include "utils/Log.h"
#include "utils/ResourceManager.h"

using IntersectionInformation = TriangleMesh::IntersectionInformation;

void Group::copyObjectsFromGroup(const Group& group) {
    for (const auto& [tag, obj] : group._attached) {
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
    auto objects = ResourceManager::loadTriangleMesh(tag, mesh_file);
    copyObjectsFromGroup(*objects);
}

void Group::add(std::shared_ptr<Object> object) {
    attach(object);
}

void Group::add(const Object &object) {
    attach(object);
}

void Group::replace(std::shared_ptr<Object> object) {
    if(find(object->name())) {
        remove(object->name());
    }
    add(object);
}

void Group::replace(const Object &object) {
    if(find(object.name())) {
        remove(object.name());
    }
    add(object);
}

void Group::add(const ObjectTag& tag, const FilePath &mesh_file, const Vec3D &scale) {
    auto objects = ResourceManager::loadTriangleMesh(tag, mesh_file);
    //objects->scale(scale);
    add(objects);
}

bool Group::remove(const ObjectTag &tag) {
    if (_attached.erase(tag) > 0) {
        unattach(tag);
        Log::log("Group::remove(): removed '" + tag.str() + "' from the group '" + name().str() + "'");
        return true;
    } else {
        for(auto& obj : _attached) {
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

std::shared_ptr<Object> Group::find(const ObjectTag &tag) {
    if (_attached.count(tag) != 0) {
        return _attached.find(tag)->second;
    }

    for(const auto& [name, obj] : _attached) {

        std::shared_ptr<Group> grObj = std::dynamic_pointer_cast<Group>(obj);
        if(grObj) {
            auto res = grObj->find(tag);
            if (res) {
                return res;
            }
        }
    }

    return nullptr;
}

IntersectionInformation Group::intersect(const Vec3D &from, const Vec3D &to, const std::set<ObjectTag> &skipTags) const {
    IntersectionInformation minIntersection;

    if(skipTags.contains(name())) {
        return minIntersection;
    }

    for(const auto& [tag, obj]: _attached) {

        if(skipTags.contains(tag)) {
            continue;
        }

        std::shared_ptr<Group> grObj = std::dynamic_pointer_cast<Group>(obj);

        IntersectionInformation intersection;
        if(grObj) {
            intersection = grObj->intersect(from, to, skipTags);
        } else {

            auto objTriangleMeshComponent = obj->getComponent<TriangleMesh>();
            if(objTriangleMeshComponent) {
                intersection = objTriangleMeshComponent->intersect(from, to);
            }
        }

        if(intersection.intersected && (intersection.distanceToObject < minIntersection.distanceToObject) && (intersection.distanceToObject > 0)) {
            minIntersection = intersection;
        }
    }

    return minIntersection;
}

void Group::clear() {
    unattachAll();
}

Group::~Group() {
    clear();
}
