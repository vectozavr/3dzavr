//
// Created by Иван Ильин on 13.01.2021.
//

#ifndef ENGINE_WORLD_H
#define ENGINE_WORLD_H

#include <map>

#include <objects/Camera.h>
#include <io/Screen.h>
#include <objects/Object.h>
#include <objects/DirectionalLight.h>
#include <physics/RigidBody.h>

class World final {
private:
    std::map<ObjectNameTag, std::shared_ptr<DirectionalLight>> _lightSources;
    std::map<ObjectNameTag, std::shared_ptr<Object>> _objects;

    void checkCollision(const ObjectNameTag &tag);
public:
    World() = default;

    void update();

    std::shared_ptr<Object> add(std::shared_ptr<Object> object);
    std::shared_ptr<Object> add(std::shared_ptr<DirectionalLight> light);
    std::shared_ptr<Object> object(const ObjectNameTag &tag);
    void remove(const ObjectNameTag &tag);
    std::shared_ptr<Mesh> loadMesh(const ObjectNameTag &tag,
                                   const std::string &mesh_file,
                                   const std::string &texture_file = "",
                                   const Vec3D &scale = Vec3D{1, 1, 1});
    void loadMap(const std::string &filename, const Vec3D &scale = Vec3D{1, 1, 1});

    // std::string skipTags is a string that consist of all objects we want to skip in ray casting
    Object::IntersectionInformation rayCast(const Vec3D &from, const Vec3D &to, const std::string &skipTags = "");

    // TODO: this function should not be here: illumination should happen by shaders, not by CPU.
    Color getIllumination(const Object::IntersectionInformation& point,
                          const Vec3D& cameraPosition,
                          const Vec3D& cameraDirection);

    std::map<ObjectNameTag, std::shared_ptr<Object>>::iterator begin() { return _objects.begin(); }
    std::map<ObjectNameTag, std::shared_ptr<Object>>::iterator end() { return _objects.end(); }
};


#endif //INC_3DZAVR_WORLD_H
