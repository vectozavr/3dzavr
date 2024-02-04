//
// Created by Иван Ильин on 13.01.2021.
//

#include <sstream>
#include <cmath>

#include <World.h>
#include <utils/Log.h>
#include <utils/ResourceManager.h>

#include <iostream>

void World::add(std::shared_ptr<Object> object) {
    _objects->add(object);
}

std::shared_ptr<Object> World::object(const ObjectTag &tag) {
    return _objects->object(tag);
}

bool World::remove(const ObjectTag &tag) {
    return _objects->remove(tag);
}

std::shared_ptr<Group> World::loadObject(const ObjectTag &tag,
                                        const FilePath &meshFile,
                                        const Vec3D &scale) {
    auto obj = ResourceManager::loadObject(tag, meshFile);
    obj->scale(scale);
    add(obj);

    Log::log("World::loadMesh(): inserted Group from " + meshFile.str() + " with title '" + tag.str() + "'");
    return obj;
}

Object::IntersectionInformation World::rayCast(const Vec3D &from, const Vec3D &to, const std::set<ObjectTag> &skipTags) {
    return _objects->rayCast(from, to, skipTags);
}

void World::update() {
    // TODO: we need to update physics state of all RigidBody inside all groups
    // so now it is incorrect and should be fixed later..
    for (auto &[name, obj] : *_objects) {
        std::shared_ptr<RigidBody> rigidBodyObj = std::dynamic_pointer_cast<RigidBody>(obj);
        if(rigidBodyObj) {
            rigidBodyObj->updatePhysicsState();
            checkCollision(name);
        }
    }
}

void World::checkCollision(const ObjectTag &tag) {

    /*
     * TODO: this is not efficient to iterate throughout all objects in the world..
     * The solution might be to use space separation (BSP-trees of something else)
     */

    // TODO: we need to check collision of all RigidBody inside all groups
    // so now it is incorrect and should be fixed later..

    std::shared_ptr<RigidBody> rigidBodyObj = std::dynamic_pointer_cast<RigidBody>(_objects->object(tag));
    if (!rigidBodyObj) {
        // The case when we cannot cast Object -> RigidBody
        return;
    }

    if (rigidBodyObj->hasCollision()) {

        rigidBodyObj->setInCollision(false);

        for (auto it = _objects->begin(); it !=  _objects->end();) {

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

World::~World() {
    _objects->clear();
}
