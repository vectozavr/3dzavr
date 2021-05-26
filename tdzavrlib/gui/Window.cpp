//
// Created by Иван Ильин on 26.03.2021.
//

#include "Window.h"

#include <utility>
#include "ResourceManager.h"

void Window::addButton(int x, int y, int w, int h, std::function<void()> click, const std::string &text, double sx, double sy,
                       const std::string &texture, tPos usualState, tPos selectedState, tPos pressedState,
                       const std::string& font, sf::Color textColor, const std::string& clickSound) {
    buttons.push_back(Button{x, y, w, h, std::move(click), text, sx, sy, texture, usualState, selectedState, pressedState, font, textColor, clickSound});
    buttons.back().init();
}

void Window::update(const std::shared_ptr<Screen>& screen) {

    screen->title(s_name);
    screen->window.draw(back);

    Point4D mousePos = screen->getMousePosition();
    Point4D dMousePos = mousePos - prevMousePosition;
    back.setPosition(back.getPosition() - sf::Vector2f(dMousePos.x/30, dMousePos.y/30));
    bool isPressed = screen->isButtonTapped(sf::Mouse::Left);

    for(auto& button : buttons) {
        if( mousePos.x > button.x - button.w*button.sx/2 && mousePos.y > button.y - button.h*button.sy/2 &&
            mousePos.x < button.x + button.w*button.sx/2 && mousePos.y < button.y + button.h*button.sy/2) {
            button.select();
            if(isPressed)
                button.press();
        } else {
            button.unSelect();
        }

        if(screen->isOpen()) {
            screen->window.draw(button.button);
            screen->window.draw(button.text);
        }
    }

    prevMousePosition = mousePos;
}

void Window::setBackgroundTexture(const std::string &texture, double sx, double sy, int w, int h) {
    s_backTexture = texture;
    sf::Texture* t = ResourceManager::loadTexture(s_backTexture);
    t->setRepeated(true);
    back = sf::Sprite(*t, sf::IntRect(0, 0, w + w/30.0, h + h/30.0));
    back.scale(sx, sy);
    back.setPosition(sf::Vector2f(-w/30.0, -h/30.0));
}
