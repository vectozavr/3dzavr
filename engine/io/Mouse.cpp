#include <io/Mouse.h>
#include <utils/Time.h>
#include <utils/Log.h>
#include <Consts.h>

Mouse *Mouse::_instance = nullptr;

Vec2D Mouse::getMousePosition() {
    int x, y;
    SDL_GetMouseState(&x, &y);
    return Vec2D(x, y);
}

Vec2D Mouse::getMouseDisplacement() {
    if (!_instance) {
        return Vec2D(0, 0);
    }

    return _instance->_motion;
}

Vec2D Mouse::getMouseScroll() {
    if (!_instance) {
        return Vec2D(0, 0);
    }

    return _instance->_scroll;
}

bool Mouse::isButtonPressed(uint8_t button) {
    if (!_instance) {
        return false;
    }

    auto it = _instance->_buttons.find(button);

    if (it != _instance->_buttons.end()) {
        return it->second;
    }

    return false;
}

bool Mouse::isButtonTapped(uint8_t button) {
    if (!_instance) {
        return false;
    }

    if (!Mouse::isButtonPressed(button)) {
        return false;
    }

    if (_instance->_tappedButtons.count(button) == 0) {
        _instance->_tappedButtons.emplace(button, Time::time());
        return true;
    } else if ((Time::time() - _instance->_tappedButtons[button]) > Consts::TAP_DELAY) {
        _instance->_tappedButtons[button] = Time::time();
        return true;
    }
    return false;
}

void Mouse::sendMouseEvent(const SDL_Event &event) {
    if (!_instance) {
        return;
    }

    switch (event.type) {
        case SDL_MOUSEMOTION:
            _instance->_motion += Vec2D(event.motion.xrel, event.motion.yrel);
            break;
        case SDL_MOUSEWHEEL:
            _instance->_scroll += Vec2D(event.wheel.x, event.wheel.y);
            break;
        case SDL_MOUSEBUTTONDOWN:
            _instance->_buttons[event.button.button] = true;
            _instance->_buttonsDown.emplace_back(event.button.button);
            break;
        case SDL_MOUSEBUTTONUP:
            _instance->_buttons[event.button.button] = false;
            _instance->_buttonsUp.emplace_back(event.button.button);
            break;
    }
}

void Mouse::init() {
    if(_instance) {
       Mouse::free();
    }

    _instance = new Mouse();

    Log::log("Mouse::init(): mouse was initialized");
}

void Mouse::free() {
    if(_instance) {
        _instance->_tappedButtons.clear();
        delete _instance;
        _instance = nullptr;
    }

    Log::log("Mouse::free(): mouse was freed");
}

void Mouse::setMouseDisplacement(const Vec2D &displacement) {
    if (!_instance) {
        return;
    }

    _instance->_motion = displacement;
}

void Mouse::setMouseScroll(const Vec2D &scroll) {
    if (!_instance) {
        return;
    }

    _instance->_scroll = scroll;
}

bool Mouse::isButtonDown() {
    if (!_instance) {
        return false;
    }

    return !_instance->_buttonsDown.empty();
}

bool Mouse::isButtonUp() {
    if (!_instance) {
        return false;
    }

    return !_instance->_buttonsUp.empty();
}

void Mouse::clear() {
    if (!_instance) {
        return;
    }

    _instance->_motion = Vec2D(0);
    _instance->_scroll = Vec2D(0);

    _instance->_buttonsDown.clear();
    _instance->_buttonsUp.clear();
}

std::vector<uint8_t> Mouse::buttonsDown() {
    if (!_instance) {
        return {};
    }

    return _instance->_buttonsDown;
}

std::vector<uint8_t> Mouse::buttonsUp() {
    if (!_instance) {
        return {};
    }

    return _instance->_buttonsUp;
}
