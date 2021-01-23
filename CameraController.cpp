//
// Created by Иван Ильин on 23.01.2021.
//

#include "CameraController.h"

CameraController::CameraController(Camera &camera, Screen &screen) : camera(camera), screen(screen) {
}

void CameraController::update() {
    // Left and right
    if (Screen::isKeyPressed(sf::Keyboard::A))
        camera.translate(camera.left()*Time::deltaTime()*5.0);

    if (Screen::isKeyPressed(sf::Keyboard::D))
        camera.translate(-camera.left()*Time::deltaTime()*5.0);

    // Forward and backward
    if (Screen::isKeyPressed(sf::Keyboard::W))
        camera.translate(camera.lookAt()*Time::deltaTime()*5.0);

    if (Screen::isKeyPressed(sf::Keyboard::S))
        camera.translate(-camera.lookAt()*Time::deltaTime()*5.0);

    if (Screen::isKeyPressed(sf::Keyboard::LShift))
        camera.translate(0.0, -Time::deltaTime()*5.0, 0);

    if (Screen::isKeyPressed(sf::Keyboard::Space))
        camera.translate(0.0, Time::deltaTime()*5.0, 0);

    // Mouse movement
    Point4D disp = screen.getMouseDisplacement();

    camera.rotateY(-disp.x/1000.0);
    camera.rotateLeft(disp.y/1000.0);
}
