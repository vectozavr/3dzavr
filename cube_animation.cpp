//
// Created by Иван Ильин on 23.01.2021.
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

TestGame::TestGame() : cameraController(external_camera, screen)
{

}

void TestGame::start() {
    // This code executed once in the beginning:

    external_camera->translate(-4, -0.5, 10);
    external_camera->rotate({0, M_PI/2.2, 0});
    external_camera->rotateLeftUpLookAt({M_PI/6, 0, 0});
//
//
    camera->setProjectionLines(true);
//
    setCameraMode(CameraMode::ExternalObserver);
//
    world->loadObj("../obj/cube.obj", "cube_1");
//
    (*world)["cube_1"]->translate(0, -3, 10);
//
    screen->setMode(Screen::ViewMode::Transparency);
    screen->setMouseCursorVisible(false);
//
    camera->translate(0, -2,5);

    //Animations:
    (*world)["cube_1"]->a_scale("a1", 1, 2, Animation::None, Animation::bouncing);
    (*world)["cube_1"]->a_decompose("a1",1, 5, Animation::None, Animation::bouncing);
    (*world)["cube_1"]->a_rotate("a1",{M_PI, M_PI, M_PI}, 5);
    (*world)["cube_1"]->a_wait("a1",0);
    (*world)["cube_1"]->a_decompose("a1",-1, 5);
    (*world)["cube_1"]->a_wait("a1",5);
//
    external_camera->a_attractToPoint("a1",(*world)["cube_1"]->position(), -3, 15);
    external_camera->a_attractToPoint("a1",camera->position(), 3, 15);
    external_camera->a_rotateRelativePoint("a1",(*world)["cube_1"]->position(), {0, -M_PI/2, 0}, 15);
    external_camera->a_rotate("a1",{0, -M_PI/6, 0}, 15);
    external_camera->a_wait("a1");
    external_camera->a_attractToPoint("a1",camera->position() + Point4D{0,-1,1}, 2, 3);
    external_camera->a_rotateLeftUpLookAt("a1",{-M_PI/6, 0, 0}, 3);
    (*world)["cube_1"]->a_wait("a1",0);
    (*world)["cube_1"]->a_rotate("a1",{M_PI, M_PI, M_PI}, 10);
    external_camera->a_wait("a1",3);
//
    external_camera->a_translateToPoint("a1",camera->position() + Point4D{5, 0, 1}, 3);
    external_camera->a_rotate("a1",{0, -M_PI/5, 0}, 3);
}

void TestGame::update(double elapsedTime) {
    // This code executed every time step:

    // Check all input after this condition please
    if (!screen->window.hasFocus())
        return;

    //cameraController.update();

    if(screen->isKeyTapped(sf::Keyboard::Escape))
        exit();
    if(screen->isKeyTapped(sf::Keyboard::E))
        switchCamera();

    if(screen->isKeyTapped(sf::Keyboard::R))
        screen->setRender(!screen->isRender());
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