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
    if (!_instance) {
        return false;
    }

    auto it = _instance->_keys.find(key);

    if (it != _instance->_keys.end()) {
        return it->second;
    }

    return false;
}

bool Keyboard::isKeyTapped(SDL_Keycode key) {
    if (!_instance) {
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
            _instance->_keysDown.emplace_back(event.key.keysym.sym);
            break;
        case SDL_KEYUP:
            _instance->_keys[event.key.keysym.sym] = false;
            _instance->_keysUp.emplace_back(event.key.keysym.sym);
            break;
        case SDL_TEXTINPUT:
            _instance->_inputText = event.text.text;
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

void Keyboard::clear() {
    if (!_instance) {
        return;
    }

    _instance->_keysDown.clear();
    _instance->_keysUp.clear();
    _instance->_inputText.clear();
}

bool Keyboard::isKeyDown() {
    if (!_instance) {
        return false;
    }

    return !_instance->_keysDown.empty();
}

bool Keyboard::isKeyUp() {
    if (!_instance) {
        return false;
    }

    return !_instance->_keysUp.empty();
}

std::vector<SDL_Keycode> Keyboard::keysDown() {
    if (!_instance) {
        return {};
    }

    return _instance->_keysDown;
}

std::vector<SDL_Keycode> Keyboard::keysUp() {
    if (!_instance) {
        return {};
    }

    return _instance->_keysUp;
}

std::string Keyboard::inputText() {
    if (!_instance) {
        return {};
    }

    return _instance->_inputText;
}
