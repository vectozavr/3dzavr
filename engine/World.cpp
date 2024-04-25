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
    /*TODO: checkCollision should happen not once per frame (update), but every fixedUpdate.
     * Maybe this check should be somehow placed into RigidObject::updatePhysicsState
     */

    checkCollision(sharedPtr());
    updateComponents();
}

void World::checkCollision(const std::shared_ptr<Object> &whereToCheck) {
    for (auto &[name, object] : *whereToCheck) {
        auto rigidObject = object->getComponent<RigidObject>();
        if(rigidObject && rigidObject->hasCollision()) {
            checkCollision(sharedPtr(), object);
            checkCollision(object);
        }
    }
}

void World::checkCollision(const std::shared_ptr<Object>& whereToCheck, const std::shared_ptr<Object>& whatToCheck) {

    for (auto &[name, object] : *whereToCheck) {
        // Check collision of whatToCheck with object
        checkCollisionBetweenTwo(whatToCheck, object);

        // Check collision of whatToCheck with all children of object
        checkCollision(object, whatToCheck);
    }
}

void World::checkCollisionBetweenTwo(const std::shared_ptr<Object> &obj1, const std::shared_ptr<Object> &obj2) {
    if(obj1->name() == obj2->name()) {
        return; // We should not check the collision of the object with itself
    }

    auto rigidObj1 = obj1->getComponent<RigidObject>();
    auto rigidObj2 = obj2->getComponent<RigidObject>();
    if(!rigidObj2) {
        return;
    }

    std::pair<bool, Simplex> gjk = GJK::checkGJKCollision(rigidObj1, rigidObj2);
    if (!gjk.first) {
        return; // no collision
    }

    auto epa = EPA::EPA(rigidObj1, rigidObj2, gjk.second);
    RigidObject::SolveCollision(epa, rigidObj1, rigidObj2);

    if (rigidObj1->collisionCallBack() != nullptr) {
        rigidObj1->collisionCallBack()(epa, rigidObj1, rigidObj2);
    }
    if (rigidObj2->collisionCallBack() != nullptr) {
        rigidObj2->collisionCallBack()(epa, rigidObj2, rigidObj1);
    }
}

