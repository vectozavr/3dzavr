//
// Created by Иван Ильин on 22.01.2022.
//

#ifndef SHOOTER_OBJECTCONTROLLER_H
#define SHOOTER_OBJECTCONTROLLER_H

#include "../Object.h"
#include "../io/Keyboard.h"
#include "../io/Mouse.h"

class ObjectController {
private:
    std::shared_ptr<Object> _object;
    std::shared_ptr<Mouse> _mouse;

public:
    ObjectController(std::shared_ptr<Object> object,
                     std::shared_ptr<Mouse> mouse);

    void update();
};

#endif //SHOOTER_OBJECTCONTROLLER_H
