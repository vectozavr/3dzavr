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
    world.loadObj("../obj/Anime_charcter.obj", "object_1", 1);
    world.loadObj("../obj/cube.obj", "object_2", 1);
    world.loadObj("../obj/cube.obj", "object_3", 1);
    world.loadObj("../obj/cube.obj", "object_4", 1);
    //world["object_1"] *= Matrix4x4::Translation(0, 0, 2);
    world["object_1"].translate(0, -3, 6);
    world["object_2"].translate(4, 0, 6);
    world["object_3"].translate(-4, 3, 6);
    world["object_4"].translate(-4, 0, 9);

    screen.setMode(Screen::ViewMode::Borders);
    screen.setMouseCursorVisible(false);

    camera.translate(0, 0, 0);
    camera.rotate(0, 0, 0);

    //external_camera.translate(65, 55, 0);
    external_camera.translate(0, 0, 0);
    external_camera.rotate(0, 0,0 );
}

void TestGame::update(double elapsedTime) {
    // This code executed every time step:

    //camera.translate(elapsedTime, 0, 0);
    //camera.rotateZ(elapsedTime*0.1);
    world["object_4"] *= Matrix4x4::Rotation({1, 1, 1}, elapsedTime);
    world["object_1"] *= Matrix4x4::RotationY(elapsedTime);
    world["object_3"] *= Matrix4x4::RotationY(elapsedTime*2);
    world["object_2"] *= Matrix4x4::RotationZ(elapsedTime/2);

    //world["object_1"] *= Matrix4x4::Translation(elapsedTime, elapsedTime, elapsedTime);

    if(cameraMode == CameraMode::LocalCamera)
        camera.keyboardControl(screen);
    else
        external_camera.keyboardControl(screen);
    //camera.keyboardControl(screen);


    //screen.keyboardControl();

    if(screen.isKeyTapped(sf::Keyboard::E)) {
        if(cameraMode == CameraMode::LocalCamera)
            setCameraMode(CameraMode::ExternalObserver);
        else
            setCameraMode(CameraMode::LocalCamera);
    }
}

int main() {
    TestGame game;
    //game.create(1920, 1080, "3dzavr test");
    game.create(3072, 1920, "3dzavr test");

    return 0;
}
