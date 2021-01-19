//
// Created by Иван Ильин on 14.01.2021.
//

#ifndef INC_3DZAVR_TDZAVR_H
#define INC_3DZAVR_TDZAVR_H

#include "Screen.h"
#include "World.h"
#include "Camera.h"
#include "Log.h"

class Tdzavr {
public:
    enum CameraMode {
        LocalCamera = 0,
        ExternalObserver
    };
protected:
    Screen screen;
    World world;
    Camera camera;
    Camera external_camera;

    double triPerSec = 0;

    CameraMode cameraMode = LocalCamera;
public:
    void create(int screenWidth = 1920, int screenHeight = 1080, const std::string& name = "3dzavr", bool verticalSync = true, sf::Color background = sf::Color(255, 255, 255));

    virtual void start() {};
    virtual void update(double elapsedTime) {};
    void exit();
    void setCameraMode(CameraMode mode);
};


#endif //INC_3DZAVR_TDZAVR_H
