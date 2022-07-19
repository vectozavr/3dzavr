//
// Created by Иван Ильин on 17.10.2021.
//

#ifndef SHOOTER_SOUNDCONTROLLER_H
#define SHOOTER_SOUNDCONTROLLER_H

#include <string>
#include <map>

#include <SFML/Audio.hpp>

class SoundTag final {
private:
    const std::string _name;
public:
    explicit SoundTag(std::string name = "") : _name(std::move(name)) {}

    [[nodiscard]] std::string str() const { return _name; }

    bool operator==(const SoundTag &tag) const { return _name == tag._name; }

    bool operator!=(const SoundTag &tag) const { return _name != tag._name; }

    bool operator<(const SoundTag &tag) const { return _name < tag._name; }
};

class SoundController final {
private:
    std::map<SoundTag, sf::Sound> _sounds;

    static SoundController *_instance;

    SoundController() = default;

public:
    SoundController(const SoundController &) = delete;

    SoundController &operator=(SoundController &) = delete;

    static void loadAndPlay(const SoundTag &soundTag, const std::string& filename);

    static void playSound(const SoundTag &soundTag);
    static void pauseSound(const SoundTag &soundTag);
    static void stopSound(const SoundTag &soundTag);

    static sf::Sound::Status getStatus(const SoundTag &soundTag);

    static void init();

    static void free();
};


#endif //SHOOTER_SOUNDCONTROLLER_H
