//
// Created by Иван Ильин on 13.01.2021.
//

#include <sstream>
#include <cmath>

#include <World.h>
#include <utils/Log.h>
#include <utils/ResourceManager.h>

#include <iostream>

World::World() {
    // Here we create the main scene, where we put all objects
    _groups.emplace(_sceneName, std::make_shared<Group>(_sceneName));
}

void World::add(std::shared_ptr<Object> object) {
    _groups[_sceneName]->add(object);
}

void World::add(std::shared_ptr<Group> group) {
    _groups.emplace(group->name(), group);
    Log::log("Group::add(): inserted group '" + group->name().str() + "'");
}

std::shared_ptr<Object> World::object(const ObjectTag &tag) {
    if (_groups[_sceneName]->objects().count(tag) != 0) {
        return _groups[_sceneName]->object(tag);
    } else if(_groups.count(tag) != 0) {
        return _groups[tag];
    }

    for(auto& gr : _groups) {
        if(gr.first == _sceneName) {
            continue; // we already checked the main group
        }

        auto grObj = gr.second->object(tag);
        if(grObj) {
            return grObj;
        }
    }

    Log::log("World::object(): no object with tag '" + tag.str() + "'");
    return nullptr;
}

bool World::remove(const ObjectTag &tag) {
    if(_groups[_sceneName]->remove(tag)) {
        return true;
    }

    for(auto& gr : _groups) {
        if(gr.first == _sceneName) {
            continue; // we already checked the main group
        }

        if(gr.second->remove(tag)) {
            return true;
        }
    }

    Log::log("World::remove(): cannot remove '" + tag.str() + "': there are no such object");
    return false;
}

std::shared_ptr<Group> World::loadObject(const ObjectTag &tag,
                                        const FileName &meshFile,
                                        const Vec3D &scale) {
    auto obj = ResourceManager::loadObject(tag, meshFile);
    obj->scale(scale);
    add(obj);

    Log::log("World::loadMesh(): inserted Group from " + meshFile.str() + " with title '" + tag.str() + "'");
    return obj;
}

Object::IntersectionInformation World::rayCast(const Vec3D &from, const Vec3D &to, const std::set<ObjectTag> &skipTags) {

    std::shared_ptr<Object::IntersectionInformation> minIntersection = std::make_shared<Object::IntersectionInformation>();

    for (const auto&[name, group]  : _groups) {
        auto intersection = group->rayCast(from, to, skipTags);

        if(intersection.distanceToObject < minIntersection->distanceToObject) {
            minIntersection = std::make_shared<Object::IntersectionInformation>(intersection);
        }
    }

    return *minIntersection;
}

void World::update() {
    // TODO: we need to update physics state of all RigidBody inside all groups
    // so now it is incorrect and should be fixed later..
    for (auto &[nameTag, gr] : _groups) {
        std::shared_ptr<RigidBody> rigidBodyObj = std::dynamic_pointer_cast<RigidBody>(gr);
        if(rigidBodyObj) {
            rigidBodyObj->updatePhysicsState();
            checkCollision(nameTag);
        }
    }
}

void World::checkCollision(const ObjectTag &tag) {

    /*
     * TODO: this is not efficient to iterate throughout all objects in the world..
     * The solution might be to use space separation.
     */

    // TODO: we need to check collision of all RigidBody inside all groups
    // so now it is incorrect and should be fixed later..

    std::shared_ptr<RigidBody> rigidBodyObj = std::dynamic_pointer_cast<RigidBody>(_groups[tag]);
    if (!rigidBodyObj) {
        // The case when we cannot cast Object -> RigidBody
        return;
    }

    if (rigidBodyObj->hasCollision()) {

        rigidBodyObj->setInCollision(false);

        for (auto it = _groups.begin(); it != _groups.end();) {

            std::shared_ptr<RigidBody> obj = std::dynamic_pointer_cast<RigidBody>(it->second);
            if (!obj) {
                // The case when we cannot cast Object -> RigidBody
                continue;
            }

            ObjectTag name = it->first;
            it++;

            if ((name == tag) || !(obj->isCollider() || obj->isTrigger())) {
                continue;
            }

            std::pair<bool, Simplex> gjk = rigidBodyObj->checkGJKCollision(obj);
            if (gjk.first) {
                if (obj->isCollider()) {
                    CollisionPoint epa = rigidBodyObj->EPA(gjk.second, obj);
                    rigidBodyObj->solveCollision(epa);
                }
                if (rigidBodyObj->collisionCallBack() != nullptr) {
                    rigidBodyObj->collisionCallBack()(name, obj);
                }
            }

        }
    }
}
