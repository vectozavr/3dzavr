//
// Created by Иван Ильин on 23.01.2021.
//

#ifndef INC_3DZAVR_CAMERACONTROLLER_H
#define INC_3DZAVR_CAMERACONTROLLER_H

#include "Camera.h"
#include "Screen.h"

class CameraController {
private:
    Camera& camera;
    Screen& screen;
public:
    CameraController(Camera& camera, Screen& screen);
    void update();
};


#endif //INC_3DZAVR_CAMERACONTROLLER_H
