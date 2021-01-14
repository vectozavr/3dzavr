//
// Created by Иван Ильин on 13.01.2021.
//

#ifndef INC_3DZAVR_WORLD_H
#define INC_3DZAVR_WORLD_H

#include <map>
#include "Mesh.h"

class World {
private:
    std::map<std::string, Mesh> internal_objects;
    std::map<std::string, Mesh&> external_objects;

public:
    World() = default;
    explicit World(const std::string& filename);

    void addMesh(Mesh& mesh, const std::string& name = "");
    void removeMesh(const std::string& name);
    void loadObj(const std::string& filename, const std::string& name = "");
};


#endif //INC_3DZAVR_WORLD_H
