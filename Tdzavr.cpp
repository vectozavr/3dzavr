//
// Created by Иван Ильин on 14.01.2021.
//

#include "Tdzavr.h"
#include "Time.h"
#include <iostream>

void Tdzavr::create(int screenWidth, int screenHeight, const std::string &name, bool verticalSync, sf::Color background) {
    screen.open(screenWidth, screenHeight, name, verticalSync, background);

    Log::log("Tdzavr::create(): started 3dzavr (" + std::to_string(screenWidth) + " x " + std::to_string(screenHeight) + ") with name '" + name + "'.");
    start();
    camera.init(screenWidth, screenHeight);
    screen.getMouseDisplacement(); // We do it to set mouse position in the center (see how getMouseDisplacement() works)

    while (screen.isOpen()) {
        screen.clear();

        Time::update();
        update(Time::deltaTime());

        screen.title(name + " [X: " + std::to_string((int)(camera.eye().x)) + " Y: " + std::to_string((int)(camera.eye().y)) + " Z: " + std::to_string((int)(camera.eye().z)) + "]");

        // project all mesh
        camera.record();
        for(auto& m : world.objects)
            camera.project(m.second, xray);

        // draw projected mesh
        for(auto& t : camera.sorted())
            screen.triangle(t.first, t.second, boundary, box);

        screen.display();
    }
    exit();
}

void Tdzavr::exit() {
    if(screen.isOpen())
        screen.close();
    Log::log("Tdzavr::exit(): exit 3dzavr (" + std::to_string(screen.width()) + " x " + std::to_string(screen.height()) + ") with name '" + screen.title() + "'.");
}
