//
// Created by Neirokan on 09.05.2020
//

#include <map>
#include <memory>
#include <sstream>
#include <fstream>

#include "ResourceManager.h"
#include "Log.h"

ResourceManager *ResourceManager::_instance = nullptr;

void ResourceManager::init() {
    delete _instance;
    _instance = new ResourceManager();

    Log::log("ResourceManager::init(): resource manager was initialized");
}

std::shared_ptr<sf::Texture> ResourceManager::loadTexture(const std::string &filename) {
    if (_instance == nullptr) {
        return nullptr;
    }

    // If texture is already loaded - return pointer to it
    auto it = _instance->_textures.find(filename);
    if (it != _instance->_textures.end()) {
        return it->second;
    }

    // Otherwise - try to load it. If failure - return zero
    std::shared_ptr<sf::Texture> texture(new sf::Texture);
    if (!texture->loadFromFile(filename)) {
        Log::log("ResourceManager::loadTexture: error with loading texture '" + filename + "'");
        return nullptr;
    }

    Log::log("ResourceManager::loadTexture: texture '" + filename + "' was loaded");

    // If success - remember and return texture pointer
    texture->setRepeated(true);
    _instance->_textures.emplace(filename, texture);

    return texture;
}

std::shared_ptr<sf::SoundBuffer> ResourceManager::loadSoundBuffer(const std::string &filename) {
    if (_instance == nullptr) {
        return nullptr;
    }

    // If sound buffer is already loaded - return pointer to it
    auto it = _instance->_soundBuffers.find(filename);
    if (it != _instance->_soundBuffers.end()) {
        return it->second;
    }

    // Otherwise - try to load it. If failure - return zero
    std::shared_ptr<sf::SoundBuffer> soundBuffer(new sf::SoundBuffer);
    if (!soundBuffer->loadFromFile(filename)) {
        Log::log("ResourceManager::loadSoundBuffer: error with loading sound buffer '" + filename + "'");
        return nullptr;
    }

    Log::log("ResourceManager::loadSoundBuffer: sound buffer '" + filename + "' was loaded");

    // If success - remember and return sound pointer
    _instance->_soundBuffers.emplace(filename, soundBuffer);

    return soundBuffer;
}

std::shared_ptr<sf::Font> ResourceManager::loadFont(const std::string &filename) {
    if (_instance == nullptr) {
        return nullptr;
    }

    // If font is already loaded - return pointer to it
    auto it = _instance->_fonts.find(filename);
    if (it != _instance->_fonts.end()) {
        return it->second;
    }

    // Otherwise - try to load it. If failure - return zero
    std::shared_ptr<sf::Font> font(new sf::Font);
    if (!font->loadFromFile(filename)) {
        Log::log("ResourceManager::loadFont: error with loading font: '" + filename + "'");
        return nullptr;
    }

    Log::log("ResourceManager::loadFont: font '" + filename + "' was loaded");

    // If success - remember and return font pointer
    _instance->_fonts.emplace(filename, font);

    return font;
}

std::vector<std::shared_ptr<Mesh>> ResourceManager::loadObjects(const std::string &filename) {

    std::vector<std::shared_ptr<Mesh>> objects{};
    std::map<std::string, sf::Color> maters{};

    if (_instance == nullptr) {
        return objects;
    }


    // If objects is already loaded - return pointer to it
    auto it = _instance->_objects.find(filename);
    if (it != _instance->_objects.end()) {
        return it->second;
    }

    std::ifstream file(filename);
    if (!file.is_open()) {
        Log::log("Mesh::LoadObjects(): cannot load file from '" + filename + "'");
        return objects;
    }

    std::vector<Vec4D> verts{};
    std::vector<Triangle> tris{};
    sf::Color currentColor = sf::Color(255, 245, 194, 255);

    while (!file.eof()) {
        char line[128];
        file.getline(line, 128);

        std::stringstream s;
        s << line;

        char junk;
        if (line[0] == 'o') {
            if (!tris.empty())
                objects.push_back(
                        std::make_shared<Mesh>(ObjectNameTag(filename + "_temp_obj_" + std::to_string(objects.size())), tris));
            tris.clear();
        }
        if (line[0] == 'v') {
            double x, y, z;
            s >> junk >> x >> y >> z;
            verts.emplace_back(x, y, z, 1.0);
        }
        if (line[0] == 'g') {
            std::string matInfo;
            s >> junk >> matInfo;
            std::string colorName = matInfo.substr(matInfo.size() - 3, 3);
            currentColor = maters[matInfo.substr(matInfo.size() - 3, 3)];
        }
        if (line[0] == 'f') {
            int f[3];
            s >> junk >> f[0] >> f[1] >> f[2];
            tris.emplace_back(verts[f[0] - 1], verts[f[1] - 1], verts[f[2] - 1], currentColor);
        }
        if (line[0] == 'm') {
            int color[4];
            std::string matName;

            s >> junk >> matName >> color[0] >> color[1] >> color[2] >> color[3];
            maters.insert({matName, sf::Color(color[0], color[1], color[2], color[3])});
        }
    }

    if (!tris.empty()) {
        objects.push_back(
                std::make_shared<Mesh>(ObjectNameTag(filename + "_temp_obj_" + std::to_string(objects.size())), tris));
    }
    tris.clear();

    file.close();

    Log::log("Mesh::LoadObjects(): obj '" + filename + "' was loaded");

    // If success - remember and return vector of objects pointer
    _instance->_objects.emplace(filename, objects);

    return objects;
}

void ResourceManager::unloadTextures() {
    if (_instance == nullptr) {
        return;
    }

    int texturesCounter = _instance->_textures.size();
    for (auto &_texture : _instance->_textures) {
        _texture.second.reset();
    }
    _instance->_textures.clear();

    Log::log("ResourceManager::unloadTextures(): all " + std::to_string(texturesCounter) + " textures was unloaded");
}

void ResourceManager::unloadSoundBuffers() {
    if (_instance == nullptr) {
        return;
    }

    int soundBuffersCounter = _instance->_soundBuffers.size();
    for (auto &_soundBuffer : _instance->_soundBuffers) {
        _soundBuffer.second.reset();
    }
    _instance->_soundBuffers.clear();

    Log::log("ResourceManager::unloadSoundBuffers(): all " + std::to_string(soundBuffersCounter) +
             " soundBuffers was unloaded");
}

void ResourceManager::unloadFonts() {
    if (_instance == nullptr) {
        return;
    }

    int fontsCounter = _instance->_fonts.size();
    for (auto &_font : _instance->_fonts) {
        _font.second.reset();
    }
    _instance->_fonts.clear();

    Log::log("ResourceManager::unloadFonts(): all " + std::to_string(fontsCounter) + " fonts was unloaded");
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
    unloadTextures();
    unloadSoundBuffers();
    unloadFonts();
    unloadObjects();

    Log::log("ResourceManager::unloadAllResources(): all resources was unloaded");
}

void ResourceManager::free() {
    unloadAllResources();

    delete _instance;
    _instance = nullptr;

    Log::log("ResourceManager::free(): pointer to 'ResourceManager' was freed");
}
