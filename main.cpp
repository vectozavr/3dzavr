#include "Tdzavr.h"
#include <iostream>

using namespace std;

class TestGame : public Tdzavr {
public:
    TestGame() = default;

    void start() override;
    void update(double elapsedTime) override;
};

void TestGame::start() {
    // This code executed once in the beginning:

    //world.addMesh(Mesh::Cube(1), "object_1");
    world.loadObj("../obj/mountains.obj", "object_1", 1);
    //Mesh triangle{};
    //Triangle t = {{1, 0, 0, 1},
    //              {-1, 0, 0, 1},
    //              {0, 1, 0, 1}};
//
    //triangle.data().push_back({t});
    //world.addMesh(triangle, "object_2");

    //world.loadObj("../obj/cube.obj", "object_2", 1);
    //world.loadObj("../obj/cube.obj", "object_3", 1);
    //world.loadObj("../obj/cube.obj", "object_4", 1);
    //world["object_1"] *= Matrix4x4::Translation(0, 0, 2);
    //world["object_1"].translate(0, -3, 6);
    world["object_1"].translate(0, -2, 6);
    //world["object_3"].translate(-4, 3, 6);
    //world["object_4"].translate(-4, 0, 9);

    screen.setMode(Screen::ViewMode::Borders);
    screen.setMouseCursorVisible(false);

    //camera.translate(-5, 22, 25);
    //camera.rotate(0, 0, 0);

    camera.translate(-5, 22, 25);
    camera.rotate(0, 0, 0);

    //external_camera.translate(-5, 22, 25);
    //external_camera.rotate(0, 0, 0);

    //external_camera.translate(65, 55, 0);
    //external_camera.translate(-2, 0, -0.5);
    //external_camera.rotate(0, M_PI/6,0 );
    external_camera.translate(68, 60, 30);
    external_camera.rotate(M_PI/5, -M_PI/2,0);
}

void TestGame::update(double elapsedTime) {
    // This code executed every time step:

    //camera.translate(elapsedTime, 0, 0);
    //camera.rotateZ(elapsedTime*0.1);
    //world["object_4"] *= Matrix4x4::Rotation({1, 1, 1}, elapsedTime);
    //world["object_1"] *= Matrix4x4::RotationY(elapsedTime);
    //world["object_3"] *= Matrix4x4::RotationY(elapsedTime*2);
    //world["object_2"] *= Matrix4x4::RotationZ(elapsedTime/2);

    //world["object_1"] *= Matrix4x4::Translation(elapsedTime, elapsedTime, elapsedTime);

    if(cameraMode == CameraMode::LocalCamera)
        camera.keyboardControl(screen);
    else
        external_camera.keyboardControl(screen);
    //camera.keyboardControl(screen);


    //screen.keyboardControl();

    camera.rotateUp(1.0/600.0);

    if(screen.isKeyTapped(sf::Keyboard::E)) {
        if(cameraMode == CameraMode::LocalCamera)
            setCameraMode(CameraMode::ExternalObserver);
        else
            setCameraMode(CameraMode::LocalCamera);
    }

    if(screen.isKeyTapped(sf::Keyboard::R))
        screen.setRender(true);
    if(screen.isKeyTapped(sf::Keyboard::T))
        screen.setRender(false);
}

int main() {
    TestGame game;
    //game.create(1920, 1080);
    game.create(1920, 1080);

    return 0;
}
