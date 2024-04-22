#ifndef ENGINE_WORLD_H
#define ENGINE_WORLD_H

#include <map>

#include <objects/Camera.h>
#include <objects/Group.h>
#include <io/Screen.h>
#include <objects/Object.h>
#include <components/physics/RigidObject.h>
#include <components/lighting/DirectionalLight.h>


class World final : public Group {
private:
    void checkCollision(const std::shared_ptr<Object>& whereToCheck);
    void checkCollision(const std::shared_ptr<Object>& whereToCheck, const std::shared_ptr<Object>& whatToCheck);
    static void checkCollisionBetweenTwo(const std::shared_ptr<Object>& obj1, const std::shared_ptr<Object>& obj2);
public:
    explicit World(const ObjectTag& sceneName) : Group(sceneName) {};

    void update();

    std::shared_ptr<Group> loadObject(const ObjectTag &tag,
                                      const FilePath &meshFile,
                                      const Vec3D &scale = Vec3D{1, 1, 1});

    // std::vector<ObjectTag> skipTags is a vector of all objects we want to skip in ray casting
    TriangleMesh::IntersectionInformation rayCast(const Vec3D &from, const Vec3D &to, const std::set<ObjectTag> &skipTags = {});
};


#endif //INC_3DZAVR_WORLD_H
