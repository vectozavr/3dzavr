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
    std::shared_ptr<Group> _objects = std::make_shared<Group>(ObjectTag("Main_Scene(World)"));
    void checkCollision(const ObjectTag &tag);
public:
    void update();

    std::shared_ptr<Object> object(const ObjectTag &tag);
    [[nodiscard]] std::shared_ptr<Group> objects() const { return _objects; }

    void add(std::shared_ptr<Object> object);
    bool remove(const ObjectTag &tag);

    std::shared_ptr<Group> loadObject(const ObjectTag &tag,
                                      const FilePath &meshFile,
                                      const Vec3D &scale = Vec3D{1, 1, 1});

    // std::vector<ObjectTag> skipTags is a vector of all objects we want to skip in ray casting
    Object::IntersectionInformation rayCast(const Vec3D &from, const Vec3D &to, const std::set<ObjectTag> &skipTags = {});

    std::map<ObjectTag, std::shared_ptr<Object>>::iterator begin() { return _objects->begin(); }
    std::map<ObjectTag, std::shared_ptr<Object>>::iterator end() { return _objects->end(); }

    ~World();
};


#endif //INC_3DZAVR_WORLD_H
