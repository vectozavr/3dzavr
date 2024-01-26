//
// Created by Иван Ильин on 13.01.2021.
//

#ifndef ENGINE_WORLD_H
#define ENGINE_WORLD_H

#include <map>

#include <objects/Camera.h>
#include <objects/Group.h>
#include <io/Screen.h>
#include <objects/Object.h>
#include <objects/DirectionalLight.h>
#include <physics/RigidBody.h>

class World final {
private:
    std::map<ObjectTag, std::shared_ptr<DirectionalLight>> _lightSources;
    std::map<ObjectTag, std::shared_ptr<Group>> _groups;
    ObjectTag _sceneName = ObjectTag("default_scene_group");

    void checkCollision(const ObjectTag &tag);
public:
    World();

    void update();

    std::shared_ptr<Object> object(const ObjectTag &tag);

    void add(std::shared_ptr<Object> object);
    void add(std::shared_ptr<Group> group);
    bool remove(const ObjectTag &tag);

    std::shared_ptr<Group> loadObject(const ObjectTag &tag,
                                      const FileName &meshFile,
                                      const Vec3D &scale = Vec3D{1, 1, 1});

    // std::vector<ObjectTag> skipTags is a vector of all objects we want to skip in ray casting
    Object::IntersectionInformation rayCast(const Vec3D &from, const Vec3D &to, const std::set<ObjectTag> &skipTags = {});

    std::map<ObjectTag, std::shared_ptr<Group>>::iterator begin() { return _groups.begin(); }
    std::map<ObjectTag, std::shared_ptr<Group>>::iterator end() { return _groups.end(); }
};


#endif //INC_3DZAVR_WORLD_H
