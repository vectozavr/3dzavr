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
    //world["object_1"] *= Matrix4x4::Translation(0, 0, 2);
    world["object_1"].translate(0, -3.5, 5);

    viewBoundary(true);
    //boxView(true);
    //xRay(true);
    screen.setMouseCursorVisible(false);
}

void TestGame::update(double elapsedTime) {
    // This code executed every time step:

    //camera.translate(elapsedTime, 0, 0);
    //camera.rotateZ(elapsedTime*0.1);
    //world["object_1"] *= Matrix4x4::Rotation({1, 1, 1}, elapsedTime);
    world["object_1"] *= Matrix4x4::RotationY(elapsedTime);
    //world["object_1"] *= Matrix4x4::Translation(elapsedTime, elapsedTime, elapsedTime);

    camera.keyboardControl(screen);
}

int main() {
    TestGame game;
    //game.create(3072, 1920, "3dzavr test");
    game.create(1920, 1080, "3dzavr test");

    return 0;
}
