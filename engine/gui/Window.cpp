//
// Created by Иван Ильин on 26.03.2021.
//

#include <utility>

#include "Window.h"
#include "../utils/ResourceManager.h"

void Window::addButton(int x, int y, int w, int h, std::function<void()> click, const std::string &text, double sx,
                       double sy,
                       const std::string &texture, tPos usualState, tPos selectedState, tPos pressedState,
                       const std::string &font, sf::Color textColor) {
    _buttons.emplace_back(x, y, w, h, std::move(click), text, sx, sy, texture, usualState, selectedState, pressedState,
                          font, textColor);
    _buttons.back().init();
}

void Window::update() {

    _screen->setTitle(_name);
    _screen->drawSprite(_back);

    Vec2D mousePos = _mouse->getMousePosition();
    Vec2D dMousePos = mousePos - _prevMousePosition;
    _prevMousePosition = mousePos;
    _back.setPosition(_back.getPosition() - sf::Vector2f(static_cast<float>(dMousePos.x()) / 30.0f,
                                                         static_cast<float>(dMousePos.y()) / 30.0f));
    bool isPressed = _mouse->isButtonTapped(sf::Mouse::Left);

    for (auto &button : _buttons) {
        if (mousePos.x() > button.x() - button.w() * button.sx() / 2.0f &&
            mousePos.y() > button.y() - button.h() * button.sy() / 2.0f &&
            mousePos.x() < button.x() + button.w() * button.sx() / 2.0f &&
            mousePos.y() < button.y() + button.h() * button.sy() / 2.0f) {
            button.select();
            if (isPressed)
                button.press();
        } else {
            button.unSelect();
        }

        if (_screen->isOpen()) {
            _screen->drawSprite(button.sprite());
            _screen->drawText(button.text());
        }
    }
}

void Window::setBackgroundTexture(const std::string &texture, double sx, double sy, int w, int h) {
    _backTexture = texture;
    std::shared_ptr<sf::Texture> t = ResourceManager::loadTexture(_backTexture);
    t->setRepeated(true);
    _back = sf::Sprite(*t, sf::IntRect(0, 0, static_cast<int>(w + w / 30.0), static_cast<int>(h + h / 30.0)));
    _back.scale((float) sx, (float) sy);
    _back.setPosition(sf::Vector2f(static_cast<float>(-w) / 30.0f, static_cast<float>(-h) / 30.0f));
}
