//
// Created by Иван Ильин on 23.02.2022.
//
#include <iostream>

#include "engine/Engine.h"
#include "engine/animation/Timeline.h"
#include "engine/animation/Animations.h"

using namespace std;

class ExampleScene : public Engine {
public:
    ExampleScene() = default;

    void start() override {
        // this code executes once in the beginning:

        auto monkey = world->loadBody(ObjectNameTag("monkey"),"obj/monkey.obj");
        monkey->translate(Vec3D{0,0,3});

        Timeline::addAnimation<AShowCreation>(monkey, 5);
        Timeline::addAnimation<AScale>(monkey, Vec3D(1.3, 1.3, 1.3), 5);
        Timeline::addAnimation<ARotate>(monkey, Vec3D{0, M_PI/4, 0}, 6, Animation::LoopOut::Continue);
        Timeline::addAnimation<AColor>(monkey, sf::Color(255, 215, 253), 10);
    }

    void update() override {
        // this code executes every frame:
        // ...
    }
};

int main(){
    ExampleScene exampleScene;

    exampleScene.create();
}