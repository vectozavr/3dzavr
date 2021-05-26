//
// Created by Иван Ильин on 13.01.2021.
//

#ifndef INC_3DZAVR_WORLD_H
#define INC_3DZAVR_WORLD_H

#include <map>
#include "Mesh.h"

class World {
private:
    std::map<std::string, std::shared_ptr<Mesh>> _objects;
public:
    World() = default;
    explicit World(const std::string& filename, const Point4D& scale = {1, 1, 1});

    [[nodiscard]] std::shared_ptr<Mesh> operator[] (const std::string& name);

    [[nodiscard]] std::map<std::string, std::shared_ptr<Mesh>>& objects() { return _objects; }

    void addMesh(const std::shared_ptr<Mesh>& mesh, const std::string& name = "");
    void removeMesh(const std::string& name);
    void loadObj(const std::string& filename, const std::string& name = "", const Point4D& scale = {1, 1, 1});

    // rayCast returns two Point4D:
    // 1) Thconst e point& of collision (the last coordinate is -1 if there are no collisions)
    // 2) Triangle of surface where the collision happened
    std::pair<Point4D, Triangle> rayCast(const Point4D& from, const Point4D& to);
};


#endif //INC_3DZAVR_WORLD_H
