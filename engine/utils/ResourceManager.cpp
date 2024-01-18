//
// Created by Neirokan on 09.05.2020
//

#include <sstream>
#include <fstream>
#include <memory>
#include <map>

#include <utils/ResourceManager.h>
#include <utils/Log.h>

ResourceManager *ResourceManager::_instance = nullptr;

void ResourceManager::init() {
    delete _instance;
    _instance = new ResourceManager();

    Log::log("ResourceManager::init(): resource manager was initialized");
}

std::vector<std::shared_ptr<Mesh>> ResourceManager::loadObjects(const std::string &mesh_file, const std::string &texture_file) {

    std::vector<std::shared_ptr<Mesh>> objects{};
    std::map<std::string, Color> maters{};

    if (_instance == nullptr) {
        return objects;
    }


    // If objects is already loaded - return pointer to it
    auto it = _instance->_objects.find(mesh_file);
    if (it != _instance->_objects.end()) {
        return it->second;
    }

    std::ifstream file(mesh_file);
    if (!file.is_open()) {
        Log::log("Mesh::LoadObjects(): cannot load file from '" + mesh_file + "'");
        return objects;
    }

    std::shared_ptr<Texture> texture = nullptr;
    if(!texture_file.empty()) {
        texture = std::make_shared<Texture>(texture_file);
    }

    std::vector<Vec4D> v{};
    std::vector<Vec3D> vt{};
    std::vector<Triangle> tris{};
    Color currentColor = Color(255, 245, 194, 255);

    while (!file.eof()) {
        std::string line;
        std::getline(file, line);

        std::stringstream lineStream;
        lineStream << line;

        std::string type;
        lineStream >> type;

        // Starting of the new object
        if (type == "o") {
            if (!tris.empty())
                objects.push_back(std::make_shared<Mesh>(
                        ObjectNameTag(mesh_file + "_temp_obj_" + std::to_string(objects.size())),
                        tris, texture));
            tris.clear();
        }
        // Vertex coordinates
        if (type == "v") {
            double x, y, z;
            lineStream >> x >> y >> z;
            v.emplace_back(x, y, z, 1.0);
        }
        // Texture coordinates
        if (type == "vt") {
            double x, y;
            lineStream >> x >> y;
            vt.emplace_back(x, y, 1.0);
        }

        // Starting of the new material
        /*
        if (type == "g") {
            std::string matInfo;
            s >> matInfo;
            std::string colorName = matInfo.substr(matInfo.size() - 3, 3);
            currentColor = maters[matInfo.substr(matInfo.size() - 3, 3)];
        }*/

        // Add a new face
        if (type == "f") {
            /*
            int f[3];
            s >> f[0] >> f[1] >> f[2];
            // TODO: we need to add separate color for each vertex
            tris.emplace_back(v[f[0] - 1], v[f[1] - 1], v[f[2] - 1],
                              std::array<Color, 3>{currentColor, currentColor, currentColor});

            */

            std::string nodes[3];
            lineStream >> nodes[0] >> nodes[1] >> nodes[2];

            int vertex[3];
            int color[3];
            // We consider only triangles (max 3 vertices)
            for(int i = 0; i < 3; i++) {
                std::stringstream s(nodes[i]);
                std::string t1, t2;

                std::getline(s, t1, '/');
                std::getline(s, t2, '/');

                vertex[i] = std::stoi(t1);

                if(!t2.empty()) {
                    color[i] = std::stoi(t2);
                }
            }

            // TODO: add texture coordinates to the triangle
            tris.emplace_back(std::array<Vec4D, 3>{v[vertex[0] - 1], v[vertex[1] - 1], v[vertex[2] - 1]},
                              std::array<Vec3D, 3>{vt[color[0] - 1], vt[color[1] - 1], vt[color[2] - 1]},
                              std::array<Color, 3>{currentColor, currentColor, currentColor});
        }
    }

    if (!tris.empty()) {
        objects.push_back(std::make_shared<Mesh>(
                ObjectNameTag(mesh_file + "_temp_obj_" + std::to_string(objects.size())),
                tris, texture));
    }
    tris.clear();
    file.close();

    Log::log("Mesh::LoadObjects(): obj '" + mesh_file + "' was loaded");

    // If success - remember and return vector of objects pointer
    _instance->_objects.emplace(mesh_file, objects);

    return objects;
}

void ResourceManager::unloadObjects() {
    if (_instance == nullptr) {
        return;
    }

    int objCounter = _instance->_objects.size();
    _instance->_objects.clear();

    Log::log("ResourceManager::unloadObjects(): all " + std::to_string(objCounter) + " objects was unloaded");
}

void ResourceManager::unloadAllResources() {
    unloadObjects();

    Log::log("ResourceManager::unloadAllResources(): all resources was unloaded");
}

void ResourceManager::free() {
    unloadAllResources();

    delete _instance;
    _instance = nullptr;

    Log::log("ResourceManager::free(): pointer to 'ResourceManager' was freed");
}
