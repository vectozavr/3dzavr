//
// Created by Иван Ильин on 23.01.2021.
//

#ifndef INC_3DZAVR_CAMERACONTROLLER_H
#define INC_3DZAVR_CAMERACONTROLLER_H

#include "Camera.h"
#include "Screen.h"

class CameraController {
private:
    std::shared_ptr<Camera> camera;
    std::shared_ptr<Screen> screen;
public:
    CameraController(std::shared_ptr<Camera>  camera, std::shared_ptr<Screen>  screen);
    void update();
};


#endif //INC_3DZAVR_CAMERACONTROLLER_H
