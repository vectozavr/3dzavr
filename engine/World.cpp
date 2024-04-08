#include <sstream>
#include <cmath>

#include <World.h>
#include <utils/Log.h>
#include <utils/ResourceManager.h>

#include <iostream>

std::shared_ptr<Group> World::loadObject(const ObjectTag &tag,
                                        const FilePath &meshFile,
                                        const Vec3D &scale) {
    auto obj = ResourceManager::loadTriangleMesh(tag, meshFile);
    obj->getComponent<TransformMatrix>()->scale(scale);
    add(obj);

    Log::log("World::loadMesh(): inserted Group from " + meshFile.str() + " with title '" + tag.str() + "'");
    return obj;
}

TriangleMesh::IntersectionInformation World::rayCast(const Vec3D &from, const Vec3D &to, const std::set<ObjectTag> &skipTags) {
    return intersect(from, to, skipTags);
}

void World::update() {
    updateComponents();

    // TODO: we need to update physics state of all RigidObject inside all groups
    // so now it is incorrect and should be fixed later..
    /*
    for (auto &[name, obj] : _attached) {
        std::shared_ptr<RigidObject> rigidBodyObj = std::dynamic_pointer_cast<RigidObject>(obj);
        if(rigidBodyObj) {
            checkCollision(name);
        }
    }
     */
}

void World::checkCollision(const ObjectTag &tag) {

    /*
     * TODO: this is not efficient to iterate throughout all objects in the world..
     * The solution might be to use space separation (BSP-trees of something else)
     */

    // TODO: we need to check collision of all RigidObject inside all groups
    // so now it is incorrect and should be fixed later..

    return;

    /*
    std::shared_ptr<RigidObject> rigidBodyObj = std::dynamic_pointer_cast<RigidObject>(_attached->find(tag));
    if (!rigidBodyObj) {
        // The case when we cannot cast Object -> RigidObject
        return;
    }

    if (rigidBodyObj->hasCollision()) {

        rigidBodyObj->setInCollision(false);

        for (auto it = _objects->begin(); it !=  _objects->end();) {

            std::shared_ptr<RigidObject> obj = std::dynamic_pointer_cast<RigidObject>(it->second);
            if (!obj) {
                // The case when we cannot cast Object -> RigidObject
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
     */
}
