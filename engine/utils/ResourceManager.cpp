//
// Created by Neirokan on 09.05.2020
//

#include <algorithm>
#include <sstream>
#include <fstream>
#include <memory>
#include <map>
#include <cmath>

#include <utils/ResourceManager.h>
#include <utils/Log.h>

ResourceManager *ResourceManager::_instance = nullptr;


void ResourceManager::init() {

    if(_instance) {
        ResourceManager::free();
    }

    _instance = new ResourceManager();

    Log::log("ResourceManager::init(): resource manager was initialized");
}

std::map<MaterialTag, std::shared_ptr<Material>> ResourceManager::loadMaterials(const FilePath &mtlFile) {

    std::map<MaterialTag, std::shared_ptr<Material>> materials;

    std::ifstream file(mtlFile.str());
    if (!file.is_open()) {
        Log::log("ResourceManager::loadMaterials(): cannot open '" + mtlFile.str() + "'");
        return materials;
    }

    // parameters of the material
    std::string matName;
    std::shared_ptr<Texture> texture = nullptr;
    Color ambient, diffuse, specular;
    uint16_t illum;
    bool readAmbient = false, readDiffuse = false, readSpecular = false, readIllum = false;

    // On each step we will check did we read all the information to be able to create a new material
    auto checkMaterialData = []
            (const std::string& matName,
                    std::shared_ptr<Texture> texture, bool readAmbient, bool readDiffuse, bool readSpecular, bool readIllum)-> bool {
        if(!matName.empty() && texture != nullptr && readAmbient && readDiffuse && readSpecular && readIllum) {
            return true;
        }
        return false;
    };

    while (!file.eof()) {
        std::string line;
        std::getline(file, line);

        std::stringstream lineStream;
        lineStream << line;

        std::string type;
        lineStream >> type;

        // Use std::transform to convert the whole string to lowercase
        std::transform(type.begin(), type.end(), type.begin(),
                       [](unsigned char c) { return std::tolower(c); });

        // material name
        if (type == "newmtl") {
            lineStream >> matName;
        }
        // illumination type
        if (type == "illum") {
            lineStream >> illum;
            readIllum = true;
        }
        // Ambient, diffuse and specular components
        if (type == "ka") {
            double r, g, b;
            lineStream >> r >> g >> b;
            ambient = Color(r * 255, g * 255, b * 255);
            readAmbient = true;
        }
        if (type == "kd") {
            double r, g, b;
            lineStream >> r >> g >> b;
            diffuse = Color(r * 255, g * 255, b * 255);
            readDiffuse = true;
        }
        if (type == "ks") {
            double r, g, b;
            lineStream >> r >> g >> b;
            specular = Color(r * 255, g * 255, b * 255);
            readSpecular = true;
        }
        if (type == "map_kd") {
            std::string textureFileName;
            lineStream >> textureFileName;
            texture = std::make_shared<Texture>(FilePath(mtlFile.parentPath(), textureFileName));
        }

        // Add a new material into the array of materials
        if(checkMaterialData(matName, texture, readAmbient, readDiffuse, readSpecular, readIllum)) {
            auto material = std::make_shared<Material>(
                    MaterialTag(matName), texture, ambient, diffuse, specular, illum);
            materials.insert({material->tag(), material});

            // When we read all data and created a new material
            // we have to clear the fields and start reading over again
            matName = "";
            texture = nullptr;
            readAmbient = false;
            readDiffuse = false;
            readSpecular = false;
            readIllum = false;
        }
    }

    return materials;
}

std::shared_ptr<Group> ResourceManager::loadObject(const ObjectTag &tag, const FilePath &objFile) {

    if (_instance == nullptr) {
        return nullptr;
    }

    std::shared_ptr<Group> objects = std::make_shared<Group>(tag);
    std::map<MaterialTag, std::shared_ptr<Material>> materials;


    // TODO: now we load and return Group of Meshes, but I would rather return the Group of Groups of Meshes

    // If objects is already loaded - return pointer to it
    auto it = _instance->_objects.find(objFile);
    if (it != _instance->_objects.end()) {
        return std::make_shared<Group>(tag, *it->second);
    }

    std::ifstream file(objFile.str());
    if (!file.is_open()) {
        Log::log("ResourceManager::loadObjects(): cannot open '" + objFile.str() + "'");
        return objects;
    }

    std::vector<Vec4D> v{};
    std::vector<Vec3D> vt{};
    std::vector<Triangle> tris{};
    std::string objName, materialName;

    std::string prevCommand;
    std::string readCommands;

    // On each step we will check did we read all the information to be able to create a new material
    auto addObject = [objects, &materials, &objName, &materialName, &tris, &readCommands] {
        if((!objName.empty() || !materialName.empty()) && !tris.empty() && !readCommands.empty()) {
            objects->add(std::make_shared<Mesh>(
                    ObjectTag(objName + "_" + materialName + "_" + std::to_string(objects->size())), tris,
                    materials[MaterialTag(materialName)]));

            // When we read all data and created a new Mesh
            // we have to clear the fields and start reading over again

            tris.clear();
            objName = "";
            materialName = "";

            readCommands = "";
        }
    };

    while (!file.eof()) {
        std::string line;
        std::getline(file, line);

        std::stringstream lineStream;
        lineStream << line;

        std::string type;
        lineStream >> type;

        // Use std::transform to convert the whole string to lowercase
        std::transform(type.begin(), type.end(), type.begin(),
                       [](unsigned char c) { return std::tolower(c); });


        if(type != prevCommand) {
            readCommands += prevCommand + "|";
        }

        if(readCommands.find(type) != std::string::npos) {
            addObject();
        }

        // Starting of the new object
        if (type == "o" || type == "g") {
            lineStream >> objName;
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

        // Add a new face
        if (type == "f") {
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

            std::array<Vec3D, 3> uv = {};
            if(!vt.empty()) {
                uv = std::array<Vec3D, 3>{vt[color[0] - 1], vt[color[1] - 1], vt[color[2] - 1]};
            }

            tris.emplace_back(std::array<Vec4D, 3>{v[vertex[0] - 1], v[vertex[1] - 1], v[vertex[2] - 1]}, uv);
        }

        // material file
        if (type == "mtllib") {
            std::string mtlFileName;
            lineStream >> mtlFileName;

            if(!mtlFileName.empty()) {
                materials = ResourceManager::loadMaterials(
                        FilePath(objFile.parentPath(), mtlFileName));
            }
        }

        // Starting of the new object
        if (type == "usemtl") {
            lineStream >> materialName;
        }

        prevCommand = type;

        if(file.eof()) {
            addObject();
        }
    }

    file.close();
    Log::log("ResourceManager::LoadObjects(): obj '" + objFile.str() + "' was loaded");

    // If success - remember and return vector of objects pointer
    _instance->_objects.emplace(objFile, objects);

    return std::make_shared<Group>(tag, *objects);
}

std::shared_ptr<Font> ResourceManager::loadFont(const FilePath &fontFile) {

    if (_instance == nullptr) {
        return nullptr;
    }

    if(_instance->_fonts.contains(fontFile) && _instance->_fonts[fontFile]) {
        return _instance->_fonts[fontFile];
    }

    std::shared_ptr<Font> font = std::make_shared<Font>(FontTag(fontFile.fileName()), fontFile);
    _instance->_fonts.insert({fontFile, font});

    return font;
}

void ResourceManager::unloadObjects() {
    if (_instance == nullptr) {
        return;
    }

    int objCounter = _instance->_objects.size();
    _instance->_objects.clear();

    Log::log("ResourceManager::unloadObjects(): all " + std::to_string(objCounter) + " objects were unloaded");
}

void ResourceManager::unloadFonts() {
    if (_instance == nullptr) {
        return;
    }

    int fontsCounter = _instance->_fonts.size();
    _instance->_fonts.clear();

    Log::log("ResourceManager::unloadFonts(): all " + std::to_string(fontsCounter) + " fonts were unloaded");
}


void ResourceManager::unloadAllResources() {
    if (_instance == nullptr) {
        return;
    }

    unloadObjects();
    unloadFonts();

    Log::log("ResourceManager::unloadAllResources(): all resources were unloaded");
}

void ResourceManager::free() {
    if(_instance) {
        unloadAllResources();

        delete _instance;
        _instance = nullptr;
    }

    Log::log("ResourceManager::free(): pointer to 'ResourceManager' was freed");
}
