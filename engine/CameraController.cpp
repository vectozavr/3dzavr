//
// Created by Иван Ильин on 22.01.2022.
//

#include "CameraController.h"
#include "utils/Time.h"
#include "Vec2D.h"

CameraController::CameraController(std::shared_ptr<Camera> camera,
                                   std::shared_ptr<Keyboard> keyboard,
                                   std::shared_ptr<Mouse> mouse) :
                                   _camera(std::move(camera)),
                                   _keyboard(std::move(keyboard)),
                                   _mouse(std::move(mouse)){
}

void CameraController::update() {
    // Left and right
    if (Keyboard::isKeyPressed(sf::Keyboard::A))
        _camera->translate(_camera->left()*Time::deltaTime()*5.0);

    if (Keyboard::isKeyPressed(sf::Keyboard::D))
        _camera->translate(-_camera->left()*Time::deltaTime()*5.0);

    // Forward and backward
    if (Keyboard::isKeyPressed(sf::Keyboard::W))
        _camera->translate(_camera->lookAt()*Time::deltaTime()*5.0);

    if (Keyboard::isKeyPressed(sf::Keyboard::S))
        _camera->translate(-_camera->lookAt()*Time::deltaTime()*5.0);

    if (Keyboard::isKeyPressed(sf::Keyboard::LShift))
        _camera->translate(Vec3D{0.0, -Time::deltaTime()*5.0, 0});

    if (Keyboard::isKeyPressed(sf::Keyboard::Space))
        _camera->translate(Vec3D{0.0, Time::deltaTime()*5.0, 0});

    // Mouse movement
    Vec2D disp = _mouse->getMouseDisplacement();

    _camera->rotate(Vec3D{0, -disp.x()/1000.0, 0});
    _camera->rotateLeft(disp.y()/1000.0);
}