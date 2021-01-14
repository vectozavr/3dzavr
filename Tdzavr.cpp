//
// Created by Иван Ильин on 14.01.2021.
//

#include "Tdzavr.h"
#include "Time.h"

void Tdzavr::create(int screenWidth, int screenHeight, const std::string &name, bool verticalSync, sf::Color background) {
    screen.open(screenWidth, screenHeight, name, verticalSync, background);

    start();

    while (screen.isOpen()) {
        screen.clear();

        Time::update();
        update(Time::deltaTime());
        // TODO: update engine


        screen.display();
    }
}
