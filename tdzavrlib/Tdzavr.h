//
// Created by Иван Ильин on 14.01.2021.
//

#ifndef INC_3DZAVR_TDZAVR_H
#define INC_3DZAVR_TDZAVR_H

#include "Screen.h"
#include "World.h"
#include "Camera.h"
#include "utils/Log.h"
#include "CameraController.h"

class Tdzavr {
public:
    enum CameraMode {
        LocalCamera = 0,
        ExternalObserver
    };
protected:
    std::shared_ptr<Screen> screen;
    std::shared_ptr<World> world;
    std::shared_ptr<Camera> camera;
    std::shared_ptr<Camera> external_camera;

    double triPerSec = 0;

    bool b_debugText = true;
    bool b_updateWorld = true;

    CameraMode cameraMode = LocalCamera;
public:
    Tdzavr();

    virtual ~Tdzavr() = default;

    void create(int screenWidth = 1920, int screenHeight = 1080, const std::string& name = "3dzavr", bool verticalSync = true, sf::Color background = sf::Color(255, 255, 255));

    virtual void start() {};
    virtual void update(double elapsedTime) {};
    void exit();
    void setCameraMode(CameraMode mode);
    void debugText(bool value) { b_debugText = value; }
    void setUpdateWorld(bool value) { b_updateWorld = value; }

    virtual void gui(){}
};


#endif //INC_3DZAVR_TDZAVR_H
