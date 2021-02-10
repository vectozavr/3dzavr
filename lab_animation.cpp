//
// Created by Иван Ильин on 27.01.2021.
//

#include "tdzavrlib/Tdzavr.h"
#include "tdzavrlib/CameraController.h"
#include <iostream>

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

    external_camera.translate(-4, -0.5, 10);
    external_camera.rotate({0, M_PI/2.2, 0});
    external_camera.rotateLeftUpLookAt({M_PI/6, 0, 0});

    world.loadObj("../obj/mountains.obj", "teapot");

    world["teapot"].translate(0, -3, 10);

    screen.setMode(Screen::ViewMode::Transparency);
    screen.setMouseCursorVisible(false);

    camera.translate(0, 10,-20);
    camera.rotateLeftUpLookAt({M_PI/6, 0, 0});

    // Animations:
    world["teapot"].a_showCreation(5);
    camera.a_rotateRelativePoint(world["teapot"].position(), {0, M_PI/4.0, 0}, 10, Animation::Continue);
    camera.a_attractToPoint(world["teapot"].position(), -10, 11);

    world["teapot"].a_decompose(10, 3);
    world["teapot"].a_wait(0);
    world["teapot"].a_decompose(-10, 3);
}

void TestGame::update(double elapsedTime) {
    // This code executed every time step:

    // Check all input after this condition please
    if (!screen.window.hasFocus())
        return;

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
