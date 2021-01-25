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

    external_camera.translate(-4, -0.5, 10);
    external_camera.rotate({0, M_PI/2.2, 0});
    external_camera.rotateUpLeftLookAt({M_PI/6, 0, 0});


    debugText(false);
    setCameraMode(CameraMode::ExternalObserver);


    //world.addMesh(Mesh::Cube(1), "object_1");
    world.loadObj("../obj/cube.obj", "cube_1", 1);
    //world.loadObj("../obj/cube.obj", "cube_2", 1);

    //world.loadObj("../obj/mountains.obj", "mountains", 1);

    //world["mountains"].translate(0, -8, 0);
    world["cube_1"].translate(0, -3, 10);
    //world["cube_2"].translate(0, -3, 20);

    screen.setMode(Screen::ViewMode::Transparency);
    screen.setMouseCursorVisible(false);

    //Mesh triangle{};
    //Triangle t = {{1, 0, 0, 1},
    //              {-1, 0, 0, 1},
    //              {0, 1, 0, 1}};
    //triangle.data().push_back({t});
    //world.addMesh(triangle, "object_2");

    camera.translate(0, -2,5);

    //camera.translateToPoint({0, 10, 0});

    // Animation
    //world["cube_1"].animation.wait(6);
    //camera.animation.translateToPoint({-11,2,0}, 5);
    //camera.animation.rotate({0, M_PI/6, 0}, 5);
    //camera.animation.wait();

    //world["cube_1"].animation.rotateRelativePoint(world["cube_2"].position(), {0, 2*M_PI, 0}, 16);
    //world["cube_1"].animation.rotate({0, 20*M_PI, 0}, 16);
    //camera.animation.rotateRelativePoint(world["cube_2"].position(), {0, 2*M_PI, 0}, 16);
    //camera.animation.attractToPoint(world["cube_2"].position(), -30, 16);
    //camera.animation.translate({0, 30, 0}, 16);
    //camera.animation.rotateUpLeftLookAt({M_PI/6, 0, 0}, 16);
    //camera.animation.wait();

    //camera.animation.attractToPoint(world["cube_2"].position(), 30, 5);
    //camera.animation.translate({0, -30, 0}, 5);
    //camera.animation.rotateUpLeftLookAt({-M_PI/6, 0, 0}, 5);

    //world["cube_1"].animation.showCreation(5);

    //world["mountains"].animation.wait(3);
    world["cube_1"].animation.showCreation(5);
    world["cube_1"].animation.wait(0);
    world["cube_1"].animation.rotate({M_PI, M_PI, M_PI}, 15);
    //camera.animation.rotateRelativePoint(world["cube_1"].position(), {0, M_PI, 0}, 20);
    //camera.animation.attractToPoint(world["cube_1"].position(), 3, 20);
    external_camera.animation.attractToPoint(world["cube_1"].position(), -3, 15);
    external_camera.animation.attractToPoint(camera.position(), 3, 15);
    external_camera.animation.rotateRelativePoint(world["cube_1"].position(), {0, -M_PI/2, 0}, 15);
    external_camera.animation.rotate({0, -M_PI/6, 0}, 15);
    external_camera.animation.wait();
    external_camera.animation.attractToPoint(camera.position() + Point4D{0,-1,1}, 2, 3);
    external_camera.animation.rotateUpLeftLookAt({-M_PI/6, 0, 0}, 3);
    world["cube_1"].animation.wait(3);
    world["cube_1"].animation.rotate({M_PI, M_PI, M_PI}, 10);
    external_camera.animation.wait(10);

    external_camera.animation.translateToPoint(camera.position() + Point4D{5, 0, 1}, 3);
    external_camera.animation.rotate({0, -M_PI/5, 0}, 3);

}

void TestGame::update(double elapsedTime) {
    // This code executed every time step:

    //cameraController.update();

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

