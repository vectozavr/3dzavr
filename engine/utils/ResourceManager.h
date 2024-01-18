//
// Created by Neirokan on 09.05.2020
//

#ifndef ENGINE_RESOURCEMANAGER_H
#define ENGINE_RESOURCEMANAGER_H

#include <memory>

#include "SDL_ttf.h"

#include "objects/geometry/Mesh.h"

class ResourceManager final {
private:
    std::map<std::string, std::vector<std::shared_ptr<Mesh>>> _objects;
    std::map<std::string, std::shared_ptr<TTF_Font>> _fonts;

    static ResourceManager *_instance;

    ResourceManager() = default;

    static void unloadObjects();

public:
    ResourceManager(const ResourceManager &) = delete;

    ResourceManager &operator=(ResourceManager &) = delete;

    static void unloadAllResources();

    static void init();

    static void free();

    // Try to load texture from file.
    // If success returns pointer to texture.
    // Otherwise, returns nullptr.
    static std::vector<std::shared_ptr<Mesh>> loadObjects(const std::string &mesh_file, const std::string &texture_file = "");

};

#endif //PSEUDO3DENGINE_RESOURCEMANAGER_H
