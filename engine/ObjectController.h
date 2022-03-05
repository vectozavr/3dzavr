//
// Created by Иван Ильин on 22.01.2022.
//

#ifndef SHOOTER_CAMERACONTROLLER_H
#define SHOOTER_CAMERACONTROLLER_H

#include "Object.h"
#include "Keyboard.h"
#include "Mouse.h"

class ObjectController {
private:
    std::shared_ptr<Object> _object;
    std::shared_ptr<Keyboard> _keyboard;
    std::shared_ptr<Mouse> _mouse;

public:
    ObjectController(std::shared_ptr<Object> object,
                     std::shared_ptr<Keyboard> keyboard,
                     std::shared_ptr<Mouse> mouse);

    void update();
};

#endif //SHOOTER_CAMERACONTROLLER_H
