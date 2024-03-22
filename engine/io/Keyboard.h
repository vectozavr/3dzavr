#ifndef IO_KEYBOARD_H
#define IO_KEYBOARD_H

#include <map>
#include <cstdint>
#include <vector>
#include <string>

#include "SDL.h"

class Keyboard final {
private:
    std::map<SDL_Keycode, bool> _keys; // 322 is the number of SDLK_DOWN events
    std::map<SDL_Keycode, double> _tappedKeys;
    std::vector<SDL_Keycode> _keysDown;
    std::vector<SDL_Keycode> _keysUp;
    std::string _inputText;

    static Keyboard *_instance;

    Keyboard() = default;
public:
    Keyboard(const Keyboard &) = delete;
    Keyboard &operator=(Keyboard &) = delete;

    // returns true if this key is _pressed
    [[nodiscard]] static bool isKeyPressed(SDL_Keycode key);
    // returns true if this key is tapped and 1/5 sec passed (_button bouncing problem solved)
    [[nodiscard]] static bool isKeyTapped(SDL_Keycode key);

    [[nodiscard]] static bool isKeyDown();
    [[nodiscard]] static bool isKeyUp();
    [[nodiscard]] static std::vector<SDL_Keycode> keysDown();
    [[nodiscard]] static std::vector<SDL_Keycode> keysUp();
    [[nodiscard]] static std::string inputText();

    static void sendKeyboardEvent(const SDL_Event& event);

    static void init();
    static void clear();
    static void free();
};


#endif //IO_KEYBOARD_H
