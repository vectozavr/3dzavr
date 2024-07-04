#ifndef UTILS_RESOURCEMANAGER_H
#define UTILS_RESOURCEMANAGER_H

#include <memory>

#include <SDL_ttf.h>

#include <components/geometry/TriangleMesh.h>
#include <objects/Group.h>
#include <utils/Font.h>

/**
 * @class ResourceManager
 * @brief ResourceManager is Singleton that manages the loading, unloading, and accessing of game resources such as 3D models and fonts.
 *
 * This class is implemented as a singleton to ensure that only one instance manages all resources
 * throughout the application. It provides methods to load and unload resources like 3D models (from .obj files)
 * and fonts, ensuring that each resource is loaded only once and reused throughout the application.
 *
 * @note The ResourceManager class is non-copyable and non-assignable.
 */
class ResourceManager final {
private:
    std::map<FilePath, std::shared_ptr<Group>> _objects; ///< Stores loaded 3D model groups indexed by file paths.
    std::map<FilePath, std::shared_ptr<Font>> _fonts; ///< Stores loaded fonts indexed by file paths.

    static ResourceManager *_instance; ///< Singleton instance of the ResourceManager.

    /**
     * @brief Default constructor. Private to prevent direct instantiation.
     */
    ResourceManager() = default;

    /**
     * @brief Unloads all loaded 3D model groups.
     */
    static void unloadObjects();

    /**
     * @brief Unloads all loaded fonts.
     */
    static void unloadFonts();

    /**
     * @brief Unloads all resources managed by the ResourceManager.
     */
    static void unloadAllResources();

    /**
     * @brief Loads materials from a material file (.mtl).
     *
     * This function is currently private and used internally by loadObjects(). It may be moved to public
     * if necessary in the future.
     *
     * @param mtlFile The file path of the material file to load.
     * @return A map of MaterialTag to shared pointers of Material, representing the loaded materials.
     */
    static std::map<MaterialTag, std::shared_ptr<Material>> loadMaterials(const FilePath &mtlFile);
public:
    ResourceManager(const ResourceManager &) = delete;
    ResourceManager &operator=(ResourceManager &) = delete;

    /**
     * @brief Initializes the ResourceManager singleton instance.
     */
    static void init();

    /**
     * @brief Frees all resources and cleans up the ResourceManager singleton instance.
     */
    static void free();

    /**
     * @brief Loads a 3D model group from an .obj file.
     *
     * This static method attempts to load a 3D model group from the specified .obj file. If successful, it returns
     * a shared pointer to the loaded Group, which contains the geometry and possibly materials of the model. If the
     * loading fails, it returns a nullptr. This method is useful for dynamically loading 3D content at runtime.
     *
     * @param tag A tag identifying the object for easier access and management.
     * @param meshFile The file path to the .obj file to be loaded.
     * @return A shared pointer to the loaded Group if successful, or nullptr if the loading fails.
     */
    static std::shared_ptr<Group> loadTriangleMesh(const ObjectTag &tag, const FilePath &meshFile);

    /**
     * @brief Loads a font from a file.
     *
     * This static method attempts to load a font from the specified file. If successful, it returns a shared pointer
     * to the loaded Font, which can be used for rendering text. If the loading fails, it returns a nullptr. This method
     * supports loading fonts dynamically at runtime, allowing for flexible text rendering in applications.
     *
     * @param fontFile The file path to the font file to be loaded.
     * @return A shared pointer to the loaded Font if successful, or nullptr if the loading fails.
     */
    static std::shared_ptr<Font> loadFont(const FilePath &fontFile);
};

#endif //UTILS_RESOURCEMANAGER_H
