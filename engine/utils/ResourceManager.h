#ifndef UTILS_RESOURCEMANAGER_H
#define UTILS_RESOURCEMANAGER_H

#include <memory>

#include <SDL_ttf.h>

#include <objects/geometry/Mesh.h>
#include <objects/Group.h>
#include <utils/Font.h>


class ResourceManager final {
private:
    std::map<FilePath, std::shared_ptr<Group>> _objects;
    std::map<FilePath, std::shared_ptr<Font>> _fonts;

    static ResourceManager *_instance;

    ResourceManager() = default;

    static void unloadObjects();
    static void unloadFonts();
    static void unloadAllResources();

    // For now this function is only used in ResourceManager::loadObjects(), if it will be necessary
    // we can move it to the public domain.
    static std::map<MaterialTag, std::shared_ptr<Material>> loadMaterials(const FilePath &mtlFile);
public:
    ResourceManager(const ResourceManager &) = delete;

    ResourceManager &operator=(ResourceManager &) = delete;


    static void init();

    static void free();

    // This function tries to load texture from the .obj file.
    // If it succeeded - the function returns a pointer to the texture.
    // Otherwise, it returns a nullptr.
    static std::shared_ptr<Group> loadObject(const ObjectTag &tag, const FilePath &meshFile);

    static std::shared_ptr<Font> loadFont(const FilePath &fontFile);
};

#endif //UTILS_RESOURCEMANAGER_H
