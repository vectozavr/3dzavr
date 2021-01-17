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
    world.loadObj("../obj/teapot.obj", "object_1", 1);
    //world["object_1"] *= Matrix4x4::Translation(0, 0, 4);
    world["object_1"].translate(0, -2, 5);

    viewBoundary(true);
}

void TestGame::update(double elapsedTime) {
    // This code executed every time step:

    //camera.translate(elapsedTime, 0, 0);
    //camera.rotateZ(elapsedTime*0.1);
    //world["object_1"] *= Matrix4x4::Rotation({1, 1, 1}, elapsedTime);
    world["object_1"] *= Matrix4x4::RotationY(elapsedTime/10.0);
    //world["object_1"] *= Matrix4x4::Translation(0, 0, elapsedTime);
}

int main() {
    TestGame game;
    game.create(1920, 1080, "3dzavr test");

    return 0;
}
