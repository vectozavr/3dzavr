#ifndef IO_KEYBOARD_H
#define IO_KEYBOARD_H

#include <map>
#include <cstdint>
#include <map>

#include "SDL.h"

class Keyboard final {
private:
    std::map<SDL_Keycode, bool> _keys; // 322 is the number of SDLK_DOWN events
    std::map<uint8_t, double> _tappedKeys;

    static Keyboard *_instance;

    Keyboard() = default;
public:
    Keyboard(const Keyboard &) = delete;
    Keyboard &operator=(Keyboard &) = delete;

    // returns true if this key is _pressed
    static bool isKeyPressed(SDL_Keycode key);

    // returns true if this key is tapped and 1/5 sec passed (_button bouncing problem solved)
    static bool isKeyTapped(SDL_Keycode key);

    static void sendKeyboardEvent(const SDL_Event& event);

    static void init();
    static void free();
};


#endif //IO_KEYBOARD_H
