//
// Created by Иван Ильин on 22.01.2022.
//

#include "ObjectController.h"
#include "Time.h"
#include "../math/Vec2D.h"

ObjectController::ObjectController(std::shared_ptr<Object> object,
                                   std::shared_ptr<Mouse> mouse) :
                                   _object(std::move(object)),
                                   _mouse(std::move(mouse)){
}

void ObjectController::update() {
    // Left and right
    if (Keyboard::isKeyPressed(sf::Keyboard::A))
        _object->translate(_object->left()*Time::deltaTime()*5.0);

    if (Keyboard::isKeyPressed(sf::Keyboard::D))
        _object->translate(-_object->left()*Time::deltaTime()*5.0);

    // Forward and backward
    if (Keyboard::isKeyPressed(sf::Keyboard::W))
        _object->translate(_object->lookAt()*Time::deltaTime()*5.0);

    if (Keyboard::isKeyPressed(sf::Keyboard::S))
        _object->translate(-_object->lookAt()*Time::deltaTime()*5.0);

    if (Keyboard::isKeyPressed(sf::Keyboard::LShift))
        _object->translate(Vec3D{0.0, -Time::deltaTime()*5.0, 0});

    if (Keyboard::isKeyPressed(sf::Keyboard::Space))
        _object->translate(Vec3D{0.0, Time::deltaTime()*5.0, 0});

    // Mouse movement
    Vec2D disp = _mouse->getMouseDisplacement();

    _object->rotate(Vec3D{0, -disp.x()/1000.0, 0});
    _object->rotateLeft(disp.y()/1000.0);
}