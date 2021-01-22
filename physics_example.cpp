//
// Created by Иван Ильин on 23.01.2021.
//

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
    world.loadObj("../obj/cube.obj", "cube_1", 1);
    world.loadObj("../obj/cube.obj", "cube_2", 1);

    world["cube_1"].translate(0, -2, 6);
    world["cube_2"].translate(0, 2, 6);

    screen.setMode(Screen::ViewMode::Default);
    screen.setMouseCursorVisible(false);

    //camera.translate(-5, 22, 25);
    //camera.rotate(0, 0, 0);

    //camera.translate(-5, 22, 25);
    //camera.rotate(0, 0, 0);
}

void TestGame::update(double elapsedTime) {
    // This code executed every time step:

    screen.keyboardControl();
    camera.keyboardControl(screen);
}

int main() {
    TestGame game;
    //game.create(1920, 1080);
    //game.create(2048, 1152);
    game.create(3072, 1920);
    //game.create(3840, 2160);

    return 0;
}

