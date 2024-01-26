//
// Created by Neirokan on 09.05.2020
//

#ifndef ENGINE_RESOURCEMANAGER_H
#define ENGINE_RESOURCEMANAGER_H

#include <memory>

#include <SDL_ttf.h>

#include "objects/geometry/Mesh.h"
#include "objects/Group.h"


class ResourceManager final {
private:
    std::map<FileName, std::shared_ptr<Group>> _objects;

    // TODO: implement ResourceManager for TTF_Font
    std::map<FileName, std::shared_ptr<TTF_Font>> _fonts;

    static ResourceManager *_instance;

    ResourceManager() = default;

    static void unloadObjects();

    // For now this function is only used in ResourceManager::loadObjects(), if it will be necessary
    // we can move it to the public domain.
    static std::map<MaterialTag, std::shared_ptr<Material>> loadMaterials(const FileName &mtl_file);
public:
    ResourceManager(const ResourceManager &) = delete;

    ResourceManager &operator=(ResourceManager &) = delete;

    static void unloadAllResources();

    static void init();

    static void free();

    // This function tries to load texture from the file.
    // If it succeeded - the function returns a pointer to the texture.
    // Otherwise, it returns a nullptr.
    static std::shared_ptr<Group> loadObject(const ObjectTag &tag, const FileName &mesh_file);
};

#endif //PSEUDO3DENGINE_RESOURCEMANAGER_H
