//
// Created by Иван Ильин on 19.09.2021.
//

#include <io/Mouse.h>
#include <utils/Time.h>
#include <utils/Log.h>
#include <Consts.h>

Mouse *Mouse::_instance = nullptr;

Vec2D Mouse::getMousePosition() {
    // TODO: implement
    return Vec2D(0, 0);
}

Vec2D Mouse::getMouseDisplacement() {
    if (_instance == nullptr) {
        return Vec2D(0, 0);
    }

    Vec2D motion(_instance->_motion);
    _instance->_motion = Vec2D();

    return motion;
}

bool Mouse::isButtonPressed(uint8_t button) {
    // TODO: implement
    return false;
}

bool Mouse::isButtonTapped(uint8_t button) {
    if (!Mouse::isButtonPressed(button)) {
        return false;
    }

    if (_tappedButtons.count(button) == 0) {
        _tappedButtons.emplace(button, Time::time());
        return true;
    } else if ((Time::time() - _tappedButtons[button]) > Consts::TAP_DELAY) {
        _tappedButtons[button] = Time::time();
        return true;
    }
    return false;
}

void Mouse::sendMouseEvent(const SDL_Event &event) {

    if (_instance == nullptr) {
        return;
    }

    // TODO: include event.button.button == SDL_BUTTON_LEFT / SDL_BUTTON_RIGHT cases
    if (event.type == SDL_MOUSEMOTION) {
        _instance->_motion = _instance->_motion + Vec2D(event.motion.xrel, event.motion.yrel);
    }
}

void Mouse::init() {
    delete _instance;
    _instance = new Mouse();

    Log::log("Mouse::init(): mouse was initialized");
}

void Mouse::free() {
    delete _instance;
    _instance = nullptr;

    Log::log("Mouse::free(): mouse was freed");
}
