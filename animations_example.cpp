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

TestGame::TestGame() : cameraController(camera, screen)
{

}

void TestGame::start() {
    // This code executed once in the beginning:

    //world.addMesh(Mesh::Cube(1), "object_1");
    world.loadObj("../obj/cube.obj", "cube_1", 1);
    world.loadObj("../obj/cube.obj", "cube_2", 1);

    world["cube_1"].translate(0, -2, 15);
    world["cube_2"].translate(0, 2, 15);

    screen.setMode(Screen::ViewMode::Default);
    screen.setMouseCursorVisible(false);

    // Animation
    camera.animation.translate(-15, 0, 15, 5);
    camera.animation.rotate(0, M_PI/2, 0, 5);
    world["cube_1"].animation.wait(5);

    world["cube_1"].animation.translate(0, 0, 4, 4);
    world["cube_1"].animation.rotate(0, M_PI, 0, 4);
    camera.animation.wait(4);

    camera.animation.translate(10, 0, 0, 5);
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

