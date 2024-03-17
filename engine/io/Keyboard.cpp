#include <io/Keyboard.h>
#include <utils/Time.h>
#include <Consts.h>
#include <utils/Log.h>

Keyboard *Keyboard::_instance = nullptr;

void Keyboard::init() {
    if(_instance) {
        Keyboard::free();
    }

    _instance = new Keyboard();

    Log::log("Keyboard::init(): keyboard was initialized");
}

bool Keyboard::isKeyPressed(SDL_Keycode key) {

    if (_instance == nullptr) {
        return false;
    }

    auto it = _instance->_keys.find(key);

    if (it != _instance->_keys.end()) {
        return it->second;
    }

    return false;
}

bool Keyboard::isKeyTapped(SDL_Keycode key) {
    if (_instance == nullptr) {
        return false;
    }

    if (!Keyboard::isKeyPressed(key)) {
        return false;
    }

    if (_instance->_tappedKeys.count(key) == 0) {
        _instance->_tappedKeys.emplace(key, Time::time());
        return true;
    } else if ((Time::time() - _instance->_tappedKeys[key]) > Consts::TAP_DELAY) {
        _instance->_tappedKeys[key] = Time::time();
        return true;
    }
    return false;
}

void Keyboard::sendKeyboardEvent(const SDL_Event &event) {
    switch (event.type) {
        // exit if the window is closed
        case SDL_KEYDOWN:
            _instance->_keys[event.key.keysym.sym] = true;
            break;
        case SDL_KEYUP:
            _instance->_keys[event.key.keysym.sym] = false;
            break;
    }
}

void Keyboard::free() {
    if(_instance) {
        _instance->_keys.clear();
        _instance->_tappedKeys.clear();
        delete _instance;

        _instance = nullptr;
    }

    Log::log("Keyboard::free(): keyboard was freed");
}
