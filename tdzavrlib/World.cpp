//
// Created by Иван Ильин on 13.01.2021.
//

#include <fstream>
#include "World.h"
#include "utils/Log.h"
#include "Plane.h"

using namespace std;

World::World(const std::string &filename, const Point4D& scale) {
    loadObj(filename, "map", scale);
    Log::log("World::World(): loaded map from " + filename + "' with " + std::to_string(_objects["map"]->triangles().size()) + " tris.");
}

void World::addMesh(const std::shared_ptr<Mesh>& mesh, const string &name) {
    _objects.emplace(name, mesh);
    Log::log("World::addMesh(): inserted mesh '" + name + "' with " + std::to_string(_objects[name]->triangles().size()) + " tris.");

}

void World::loadObj(const string &filename, const string &name, const Point4D& scale) {
    std::shared_ptr<Mesh> resMesh = std::make_shared<Mesh>(filename);

    resMesh->scale(scale);
    _objects.emplace(name, resMesh);
    Log::log("World::loadObj(): inserted mesh from " + filename + " with name '" + name + "' with " + std::to_string(_objects[name]->triangles().size()) + " tris.");
}

void World::removeMesh(const string &name) {
    if(_objects.erase(name) > 0)
        Log::log("World::removeMesh(): removed mesh '" + name + "'");
    else
        Log::log("World::removeMesh(): cannot remove mesh '" + name + "': mesh does not exist.");
}

std::shared_ptr<Mesh> World::operator[](const string &name) {
    if(_objects.count(name) == 0)
        Log::log("World::operator[]: mesh '" + name + "' does not exist.");
    return _objects.find(name)->second;
}

std::pair<Point4D, Triangle> World::rayCast(const Point4D& from, const Point4D& to) {

    std::pair<Point4D, Triangle> result{Point4D{0, 0,0, -1}, Triangle{}};
    double minDistance = 10000;

    for(auto& object : _objects) {
        if((object.first.find("Player") != std::string::npos) || (object.first.find("point") != std::string::npos))
            continue;

        for(auto& tri : object.second->triangles()) {
            Triangle tri_translated(tri[0] + object.second->position(), tri[1] + object.second->position(), tri[2] + object.second->position());
            tri_translated[0].w = 0; tri_translated[1].w = 0; tri_translated[2].w = 0;

            Plane plane(tri_translated);
            auto intersection = plane.intersection(from, to);
            //cout << intersection.second << endl;
            double distance = (intersection.first - from).sqrAbs();
            if(intersection.second > 0 && distance < minDistance && tri_translated.isPointInside(intersection.first)) {
                minDistance = distance;
                result = {intersection.first, tri_translated};
                //tri.color = {255, 0, 0};
            } else {
                //tri.color = {0, 255, 0};
            }
        }
    }
    return result;
}
