//
// Created by Иван Ильин on 13.01.2021.
//

#ifndef INC_3DZAVR_WORLD_H
#define INC_3DZAVR_WORLD_H

#include <map>
#include "Mesh.h"

class World {
public:
    std::map<std::string, Mesh> objects;

    World() = default;
    explicit World(const std::string& filename, Point4D scale = {1, 1, 1});

    [[nodiscard]] Mesh& operator[] (const std::string& name);

    void addMesh(Mesh& mesh, const std::string& name = "");
    void copyMesh(const std::string& meshName, const std::string& copyName);
    void addMesh(const Mesh& mesh, const std::string& name = "");
    void removeMesh(const std::string& name);
    void loadObj(const std::string& filename, const std::string& name = "", Point4D scale = {1, 1, 1});
};


#endif //INC_3DZAVR_WORLD_H
