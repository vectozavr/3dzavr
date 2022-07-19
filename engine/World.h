//
// Created by Иван Ильин on 13.01.2021.
//

#ifndef ENGINE_WORLD_H
#define ENGINE_WORLD_H

#include <map>

#include "Camera.h"
#include "io/Screen.h"
#include "physics/RigidBody.h"

struct IntersectionInformation final {
    const Vec3D pointOfIntersection;
    const double distanceToObject;
    const Triangle intersectedTriangle;
    const ObjectNameTag objectName;
    const std::shared_ptr<RigidBody> obj;
    const bool intersected;
};

class World final {
private:
    std::map<ObjectNameTag, std::shared_ptr<RigidBody>> _objects;

    void checkCollision(const ObjectNameTag &tag);
public:
    World() = default;

    void update();

    std::shared_ptr<RigidBody> addBody(std::shared_ptr<RigidBody> mesh);
    std::shared_ptr<RigidBody> body(const ObjectNameTag &tag);
    void removeBody(const ObjectNameTag &tag);
    std::shared_ptr<RigidBody> loadBody(const ObjectNameTag &tag, const std::string &filename, const Vec3D &scale = Vec3D{1, 1, 1});
    void loadMap(const std::string &filename, const Vec3D &scale = Vec3D{1, 1, 1});

    // std::string skipTags is a string that consist of all objects we want to skip in ray casting
    IntersectionInformation rayCast(const Vec3D &from, const Vec3D &to, const std::string &skipTags = "");

    std::map<ObjectNameTag, std::shared_ptr<RigidBody>>::iterator begin() { return _objects.begin(); }
    std::map<ObjectNameTag, std::shared_ptr<RigidBody>>::iterator end() { return _objects.end(); }
};


#endif //INC_3DZAVR_WORLD_H
