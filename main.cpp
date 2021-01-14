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

    cout << "START!" << endl;
    viewBoundary(true);
}

void TestGame::update(double elapsedTime) {
    // This code executed every time step:

    cout << "UPDATE!" << endl;
}

int main() {
    TestGame game;
    game.create(1920, 1080, "3dzavr test");

    return 0;
}
