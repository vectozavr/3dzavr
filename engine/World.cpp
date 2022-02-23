//
// Created by Иван Ильин on 13.01.2021.
//

#include <sstream>
#include <cmath>

#include "World.h"
#include "utils/Log.h"
#include "Plane.h"
#include "ResourceManager.h"

using namespace std;

void World::addBody(std::shared_ptr<RigidBody> body) {
    _objects.emplace(body->name(), body);
    Log::log("World::addBody(): inserted body '" + body->name().str() + "' with " +
             std::to_string(_objects[body->name()]->triangles().size()) + " tris.");
}

void World::loadBody(const ObjectNameTag &tag, const string &filename, const Vec3D &scale) {
    _objects.emplace(tag, std::make_shared<RigidBody>(tag, filename, scale));
    Log::log("World::loadBody(): inserted body from " + filename + " with title '" + tag.str() + "' with " +
             std::to_string(_objects[tag]->triangles().size()) + " tris.");
}

IntersectionInformation World::rayCast(const Vec3D &from, const Vec3D &to, const std::string &skipTags) {

    // make vector of tags, that we are going to escape
    vector<std::string> tagsToSkip;
    stringstream s(skipTags);
    std::string t;
    while (s >> t) {
        tagsToSkip.push_back(t);
    }

    bool intersected = false;
    Vec3D point{};
    Triangle triangle;
    std::string bodyName;
    double minDistance = Consts::RAY_CAST_MAX_DISTANCE;
    std::shared_ptr<RigidBody> intersectedBody = nullptr;

    for (auto&[name, body]  : _objects) {

        bool escapeThisBody = false;
        for (auto &escapeTag : tagsToSkip) {
            if (name.contains(ObjectNameTag(escapeTag))) {
                escapeThisBody = true;
                break;
            }
        }
        if (escapeThisBody) {
            continue;
        }

        for (auto &tri : body->triangles()) {
            Matrix4x4 model = body->model();
            Triangle tri_translated(model * tri[0], model * tri[1], model * tri[2], tri.color());

            Plane plane(tri_translated);
            auto intersection = plane.intersection(from, to);
            double distance = (intersection.first - from).sqrAbs();
            if (intersection.second > 0 && distance < minDistance && tri_translated.isPointInside(intersection.first)) {
                minDistance = distance;
                point = intersection.first;
                triangle = tri_translated;
                bodyName = name.str();
                intersected = true;
                intersectedBody = body;
            }
        }
    }
    return IntersectionInformation{point, sqrt(minDistance), triangle, ObjectNameTag(bodyName), intersectedBody,
                                   intersected};
}

void World::loadMap(const std::string &filename, const Vec3D &scale) {
    auto objs = ResourceManager::loadObjects(filename);
    for (auto &i : objs) {
        std::shared_ptr<RigidBody> obj = std::make_shared<RigidBody>(*i, false);
        addBody(obj);
        obj->scale(scale);
    }
}

void World::removeBody(const ObjectNameTag &tag) {
    if (_objects.erase(tag) > 0) {
        Log::log("World::removeBody(): removed body '" + tag.str() + "'");
    } else {
        Log::log("World::removeBody(): cannot remove body '" + tag.str() + "': body does not exist.");
    }
}

void World::checkCollision(const ObjectNameTag &tag) {
    if (_objects[tag]->hasCollision()) {

        _objects[tag]->setInCollision(false);

        for (auto it = _objects.begin(); it != _objects.end();) {
            auto obj = it->second;
            ObjectNameTag name = it->first;
            it++;

            if ((name == tag) || !(obj->isCollider() || obj->isTrigger())) {
                continue;
            }

            std::pair<bool, Simplex> gjk = _objects[tag]->checkGJKCollision(obj);

            if (gjk.first) {
                if (obj->isCollider()) {
                    CollisionPoint epa = _objects[tag]->EPA(gjk.second, obj);
                    _objects[tag]->solveCollision(epa);
                }
                if (_objects[tag]->collisionCallBack() != nullptr) {
                    _objects[tag]->collisionCallBack()(name, obj);
                }
            }

        }
    }
}

void World::update() {
    for (auto &[nameTag, obj] : _objects) {
        obj->updatePhysicsState();
        checkCollision(nameTag);
    }
}

std::shared_ptr<RigidBody> World::body(const ObjectNameTag &tag) {
    if (_objects.count(tag) == 0) {
        return nullptr;
    }
    return _objects.find(tag)->second;
}
