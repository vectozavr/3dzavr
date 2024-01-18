//
// Created by Иван Ильин on 22.01.2022.
//

#ifndef SHOOTER_OBJECTCONTROLLER_H
#define SHOOTER_OBJECTCONTROLLER_H

#include "objects/Object.h"
#include <io/Keyboard.h>
#include <io/Mouse.h>

class ObjectController {
private:
    std::shared_ptr<Object> _object;

public:
    ObjectController(std::shared_ptr<Object> object);

    void update();
};

#endif //SHOOTER_OBJECTCONTROLLER_H
