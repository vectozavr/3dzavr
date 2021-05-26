//
// Created by Иван Ильин on 26.03.2021.
//

#include "Button.h"
#include "../ResourceManager.h"

void Button::select()
{
    if (!selected && !pressed)
    {
        button.setTextureRect(sf::IntRect(selectedState.tx, selectedState.ty, w, h));
        selected = true;
    }
}

void Button::unSelect()
{
    if (selected && !pressed)
    {
        button.setTextureRect(sf::IntRect(usualState.tx, usualState.ty, w, h));
        selected = false;
    }
}

void Button::press()
{
    if (!pressed)
    {
        button.setTextureRect(sf::IntRect(pressedState.tx, pressedState.ty, w, h));
        if(checkBox)
            pressed = true;
        clickSound.play();
        click();
    }
    else
    {
        button.setTextureRect(sf::IntRect(usualState.tx, usualState.ty, w, h));
        if(checkBox)
            pressed = false;
    }
}

void Button::init() {
    button.setTexture(*ResourceManager::loadTexture(s_texture));
    button.setTextureRect(sf::IntRect(usualState.tx, usualState.ty, w, h));
    button.scale(sx, sy);
    button.setPosition(x - w*sx/2, y - h*sy/2);

    text.setFont(*ResourceManager::loadFont(s_font));
    text.setString(s_text);
    text.setCharacterSize(h*sy/2);
    text.setFillColor(textColor);
    text.setPosition(x - text.getLocalBounds().width/2, y - h*sy/2 + text.getLocalBounds().height/4);

    clickSound.setBuffer(*ResourceManager::loadSoundBuffer(s_clickSound));
    clickSound.setVolume(15);
}
