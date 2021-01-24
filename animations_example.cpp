//
// Created by Иван Ильин on 23.01.2021.
//

#include "Tdzavr.h"
#include "CameraController.h"
#include <iostream>
#include "Animation.h"

using namespace std;

class TestGame : public Tdzavr {
private:
    CameraController cameraController;

public:
    TestGame();

    void start() override;
    void update(double elapsedTime) override;

    void switchCamera();
};

TestGame::TestGame() : cameraController(external_camera, screen)
{

}

void TestGame::start() {
    // This code executed once in the beginning:

    external_camera.translate(-60, 40, -12);
    debugText(false);

    //world.addMesh(Mesh::Cube(1), "object_1");
    world.loadObj("../obj/cube.obj", "cube_1", 1);
    world.loadObj("../obj/cube.obj", "cube_2", 1);
    world.loadObj("../obj/mountains.obj", "mountains", 1);

    world["mountains"].translate(0, -8, 0);
    world["cube_1"].translate(0, -3, 10);
    world["cube_2"].translate(0, -3, 20);

    screen.setMode(Screen::ViewMode::Default);
    screen.setMouseCursorVisible(false);


    camera.translateToPoint({0, 20, 0});

    // Animation
    world["cube_1"].animation.wait(6);
    camera.animation.translateToPoint({-11,2,0}, 5);
    camera.animation.rotate({0, M_PI/6, 0}, 5);
    camera.animation.wait();

    world["cube_1"].animation.rotateRelativePoint(world["cube_2"].position(), {0, 2*M_PI, 0}, 16);
    world["cube_1"].animation.rotate({0, 20*M_PI, 0}, 16);
    camera.animation.rotateRelativePoint(world["cube_2"].position(), {0, 2*M_PI, 0}, 16);
    camera.animation.attractToPoint(world["cube_2"].position(), -30, 16);
    camera.animation.translate({0, 30, 0}, 16);
    camera.animation.rotateUpLeftLookAt({M_PI/6, 0, 0}, 16);
    camera.animation.wait();

    camera.animation.attractToPoint(world["cube_2"].position(), 30, 5);
    camera.animation.translate({0, -30, 0}, 5);
    camera.animation.rotateUpLeftLookAt({-M_PI/6, 0, 0}, 5);

}

void TestGame::update(double elapsedTime) {
    // This code executed every time step:

    cameraController.update();

    if(screen.isKeyTapped(sf::Keyboard::Escape))
        exit();
    if(screen.isKeyTapped(sf::Keyboard::E))
        switchCamera();
}

void TestGame::switchCamera() {
    if(cameraMode == CameraMode::LocalCamera)
        setCameraMode(CameraMode::ExternalObserver);
    else
        setCameraMode(CameraMode::LocalCamera);
}

int main() {
    TestGame game;
    //game.create(1920, 1080);
    //game.create(2048, 1152);
    game.create(3072, 1920);
    //game.create(3840, 2160);

    return 0;
}

