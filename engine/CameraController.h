//
// Created by Иван Ильин on 22.01.2022.
//

#ifndef SHOOTER_CAMERACONTROLLER_H
#define SHOOTER_CAMERACONTROLLER_H

#include "Camera.h"
#include "Keyboard.h"
#include "Mouse.h"

class CameraController {
private:
    std::shared_ptr<Camera> _camera;
    std::shared_ptr<Keyboard> _keyboard;
    std::shared_ptr<Mouse> _mouse;

public:
    CameraController(std::shared_ptr<Camera> camera,
                     std::shared_ptr<Keyboard> keyboard,
                     std::shared_ptr<Mouse> mouse);

    void update();
};

#endif //SHOOTER_CAMERACONTROLLER_H
