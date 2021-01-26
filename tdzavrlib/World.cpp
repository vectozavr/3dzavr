//
// Created by Иван Ильин on 13.01.2021.
//

#include <fstream>
#include "World.h"
#include "utils/Log.h"

using namespace std;

World::World(const std::string &filename, double scale) {
    loadObj(filename, "map", scale);
    Log::log("World::World(): loaded map from " + filename + "' with " + std::to_string(objects["map"].triangles().size()) + " tris.");
}

void World::addMesh(Mesh &mesh, const string &name) {
    objects.emplace(name, std::move(mesh));
    Log::log("World::addMesh(): inserted lvalue mesh '" + name + "' with " + std::to_string(objects[name].triangles().size()) + " tris.");

}

void World::addMesh(const Mesh& mesh, const string &name) {
    objects.emplace(name, mesh);
    Log::log("World::addMesh(): inserted rvalue mesh '" + name + "' with " + std::to_string(objects[name].triangles().size()) + " tris.");
}

void World::loadObj(const string &filename, const string &name, double scale) {
    objects.emplace(name, Mesh(filename)*Matrix4x4::Scale(scale, scale, scale));
    Log::log("World::loadObj(): inserted mesh from " + filename + " with name '" + name + "' with " + std::to_string(objects[name].triangles().size()) + " tris.");
}

void World::removeMesh(const string &name) {
    if(objects.erase(name) > 0)
        Log::log("World::removeMesh(): removed mesh '" + name + "'");
    else
        Log::log("World::removeMesh(): cannot remove mesh '" + name + "': mesh does not exist.");
}

Mesh &World::operator[](const string &name) {
    if(objects.count(name) == 0)
        Log::log("World::operator[]: mesh '" + name + "' does not exist.");
    return objects.find(name)->second;
}

void World::copyMesh(const string &meshName, const string &copyName) {
    objects.emplace(copyName, objects[meshName]);
}
