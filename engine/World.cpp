//
// Created by Иван Ильин on 13.01.2021.
//

#include <sstream>
#include <cmath>

#include "World.h"
#include "utils/Log.h"
#include "math/Plane.h"
#include "utils/ResourceManager.h"

using namespace std;

std::shared_ptr<RigidBody> World::addBody(std::shared_ptr<RigidBody> body) {
    _objects.emplace(body->name(), body);
    Log::log("World::addBody(): inserted body '" + body->name().str() + "' with " +
             std::to_string(_objects[body->name()]->triangles().size()) + " tris.");
    return _objects[body->name()];
}

std::shared_ptr<RigidBody> World::loadBody(const ObjectNameTag &tag, const string &filename, const Vec3D &scale) {
    _objects.emplace(tag, std::make_shared<RigidBody>(tag, filename, scale));
    Log::log("World::loadBody(): inserted body from " + filename + " with title '" + tag.str() + "' with " +
             std::to_string(_objects[tag]->triangles().size()) + " tris.");
    return _objects[tag];
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

        Matrix4x4 model = body->model();
        // It is computationally more efficient not to transform all object's triangles from model to global
        // coordinate system, but translate 'from' and 'to' vectors inside once and check triangles without performing
        // many matrix multiplication.
        Matrix4x4 invModel = body->invModel();

        Vec3D v = (to - from).normalized();
        Vec3D v_model = invModel*v;
        Vec3D from_model = invModel*(from - body->position());
        Vec3D to_model = invModel*(to - body->position());


        for (auto &tri : body->triangles()) {

            if(tri.norm().dot(v_model) > 0) {
                continue;
            }

            auto intersection = Plane(tri).intersection(from_model, to_model);

            if (intersection.second > 0 && tri.isPointInside(intersection.first)) {

                // When you change to model coordinate system you also will get distance scaled by invModel.
                // Due-to this effect if you scale some object in x times you will get distance in x times smaller.
                // That's why we need to perform distance calculation in the global coordinate system where metric
                // is the same for all objects.
                Triangle globalTriangle(model * tri[0], model * tri[1], model * tri[2], tri.color());
                auto globalIntersection = Plane(globalTriangle).intersection(from, to);
                double globalDistance = (globalIntersection.first - from).abs();

                if(globalDistance < minDistance) {
                    minDistance = globalDistance;
                    point = globalIntersection.first;
                    triangle = globalTriangle;
                    bodyName = name.str();
                    intersected = true;
                    intersectedBody = body;
                    //Triangle triangleRED = Triangle(model * tri[0], model * tri[1], model * tri[2], sf::Color(255, 0, 0));
                    //addBody(std::make_shared<RigidBody>(Mesh(ObjectNameTag("Test" + std::to_string(rand())), std::vector<Triangle>({triangleRED}))));
                }
            }
        }
    }

    return IntersectionInformation{point, sqrt(minDistance), triangle, ObjectNameTag(bodyName), intersectedBody, intersected};
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
