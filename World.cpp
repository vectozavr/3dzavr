//
// Created by Иван Ильин on 13.01.2021.
//

#include <fstream>
#include "World.h"
#include "Log.h"

using namespace std;

World::World(const std::string &filename) {
    ifstream file(filename);

    if(filename.empty()) {
        Log::log("World: Cannot load map from empty filename");
        return;
    } else if (!file.is_open())
    {
        Log::log("World: Cannot load mesh from " + filename);
        return;
    }

    // TODO: implement map loading

    file.close();
}

void World::addMesh(Mesh &mesh, const string &name) {
    external_objects.insert({ name, mesh });
}

void World::loadObj(const string &filename, const string &name) {
    internal_objects.insert({name, Mesh(filename)});
}

void World::removeMesh(const string &name) {
    internal_objects.erase(name);
    external_objects.erase(name);
}
