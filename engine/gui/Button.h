//
// Created by Иван Ильин on 26.03.2021.
//

#ifndef ENGINE_BUTTON_H
#define ENGINE_BUTTON_H

#include <functional>

#include <SFML/Graphics.hpp>

struct tPos final {
    const int tx;
    const int ty;
};

class Button final {
private:
    const int _x{};
    const int _y{};

    const int _w{};
    const int _h{};

    const std::function<void()> _click;

    const std::string _textString;

    const double _sx{};
    const double _sy{};

    const std::string _texture;
    const tPos _usualState{};
    const tPos _selectedState{};
    const tPos _pressedState{};

    const std::string _font;
    const sf::Color _textColor;

    sf::Sprite _button;
    sf::Text _text;

    bool _selected = false;
    bool _pressed = false;
    bool _checkBox = false;

public:
    Button() = default;

    Button(int x, int y, int width, int height, std::function<void()> click, std::string text, double sx, double sy,
           std::string texture, tPos usualState, tPos selectedState, tPos pressedState, std::string font,
           sf::Color textColor);

    void select();

    void unSelect();

    void press();

    void init();

    [[nodiscard]] int x() const { return _x; }

    [[nodiscard]] int y() const { return _y; }

    [[nodiscard]] int w() const { return _w; }

    [[nodiscard]] int h() const { return _h; }

    [[nodiscard]] double sx() const { return _sx; }

    [[nodiscard]] double sy() const { return _sy; }

    [[nodiscard]] sf::Sprite const &sprite() const { return _button; }

    [[nodiscard]] sf::Text const &text() const { return _text; }
};


#endif //MINECRAFT_3DZAVR_BUTTON_H
