//
// Created by Иван Ильин on 19.09.2021.
//

#ifndef SHOOTER_MOUSE_H
#define SHOOTER_MOUSE_H

#include <memory>
#include <utility>

#include <io/Screen.h>
#include <linalg/Vec2D.h>

class Mouse final {
private:
    std::map<uint8_t, double> _tappedButtons;
    Vec2D _motion;

    static Mouse *_instance;
    Mouse() = default;
public:

    // returns true if this _button is _pressed
    static bool isButtonPressed(uint8_t button);

    // returns true if this _button is tapped and 1/5 sec passed (_button bouncing problem solved)
    bool isButtonTapped(uint8_t button);

    [[nodiscard]] static Vec2D getMousePosition();
    [[nodiscard]] static Vec2D getMouseDisplacement();

    static void sendMouseEvent(const SDL_Event& event);

    static void init();
    static void free();
};


#endif //SHOOTER_MOUSE_H
