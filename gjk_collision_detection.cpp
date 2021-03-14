//
// Created by Иван Ильин on 06.02.2021.
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

    screen.setMode(Screen::ViewMode::Transparency);
    screen.setMouseCursorVisible(false);
    camera.translate(0, 0, -10);

    world.loadObj("../obj/cube.obj", "cube_1");
    world.loadObj("../obj/cube.obj", "cube_2");

    world["cube_1"].translate(-0.5, -1, 0.5);
    world["cube_2"].translate(0, 5, 0);

    world["cube_2"].setCollision(true);
    world["cube_2"].setDebugMode(true);

    world["cube_2"].applyAcceleration({0, -9.81, 0});
    //world["cube_2"].rotate({M_PI/4, 0, M_PI/4});
    //world["cube_2"].rotate({0, 0, 0.001});

    world["cube_1"].a_translate({0, 6, 0}, 5);
    world["cube_1"].a_wait(0);
    world["cube_1"].a_translate({0, -6, 0}, 5);


    //vector<Triangle>tris;
    //tris.push_back(Triangle(Point4D{0,0,-1,1},Point4D{0, 0, 1, 1}, Point4D{0, 1, 0, 1}));
    //Mesh triangle(tris);
    //world.addMesh(triangle, "t1");
    //world.addMesh(triangle, "t2");
    ////world["t1"].rotate({0, 0, -M_PI/6});
    ////world["t1"].scale({2.5, 2.5, 2.5});
    ////world["t2"].rotate({M_PI/6, M_PI/2, 0});
    //world["t1"].translate(0, 3, 5);
    //world["t2"].translate(0, 1, 5);
    //world["t2"].setCollision(true);
    //world["t2"].setDebugMode(true);

    //world["t1"].a_translate({0, -1.5, 0}, 3);
    //world["t1"].a_wait(0);
    //world["t1"].a_translate({1.5, 0, 0}, 3);
}

void TestGame::update(double elapsedTime) {
    // This code executed every time step:

    cameraController.update();

    if(screen.isKeyTapped(sf::Keyboard::Escape))
        exit();
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