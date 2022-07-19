//
// Created by Neirokan on 09.05.2020
//

#ifndef ENGINE_RESOURCEMANAGER_H
#define ENGINE_RESOURCEMANAGER_H

#include <memory>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "../Mesh.h"

class ResourceManager final {
private:
    std::map<std::string, std::shared_ptr<sf::Texture>> _textures;
    std::map<std::string, std::shared_ptr<sf::Font>> _fonts;
    std::map<std::string, std::shared_ptr<sf::SoundBuffer>> _soundBuffers;
    std::map<std::string, std::vector<std::shared_ptr<Mesh>>> _objects;

    static ResourceManager *_instance;

    ResourceManager() = default;

    // Unloads all currently loaded textures.
    static void unloadObjects();

    static void unloadTextures();

    static void unloadSoundBuffers();

    static void unloadFonts();

public:
    ResourceManager(const ResourceManager &) = delete;

    ResourceManager &operator=(ResourceManager &) = delete;

    static void unloadAllResources();

    static void init();

    static void free();

    // Try to load texture from file.
    // If success returns pointer to texture.
    // Otherwise returns nullptr.
    static std::vector<std::shared_ptr<Mesh>> loadObjects(const std::string &filename);

    static std::shared_ptr<sf::Texture> loadTexture(const std::string &filename);

    static std::shared_ptr<sf::Font> loadFont(const std::string &filename);

    static std::shared_ptr<sf::SoundBuffer> loadSoundBuffer(const std::string &filename);
};

#endif //PSEUDO3DENGINE_RESOURCEMANAGER_H
