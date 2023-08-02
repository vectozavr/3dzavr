//
// Created by Иван Ильин on 23.02.2022.
//

#include <Engine.h>
#include <animation/Timeline.h>
#include <animation/Animations.h>

class ExampleScene : public Engine {
public:
    ExampleScene() = default;

    void start() override {
        // this code executes once in the beginning:
        auto monkey = world->loadBody(ObjectNameTag("monkey"),"obj/monkey.obj");
        monkey->translate(Vec3D{0,0,3});

        Timeline::addAnimation<AShowCreation>(monkey, 3); // add creation animation
        Timeline::addAnimation<AWait>(0); // wait before start next animation
        Timeline::addAnimation<ADecompose>(monkey, 1); // add expansion animation
        Timeline::addAnimation<AWait>(0); // wait before start next animation
        Timeline::addAnimation<ADecompose>(monkey, -1); // reverse expansion
        Timeline::addAnimation<AScale>(monkey, Vec3D(1.3, 1.3, 1.3), 5); // add scale expansion
        Timeline::addAnimation<ARotate>(monkey, Vec3D{0, Consts::PI/4, 0}, 6, Animation::LoopOut::Continue); // add rotation
        Timeline::addAnimation<AColor>(monkey, sf::Color(255, 215, 253), 10); // add color animation
    }

    void update() override {
        // this code executes every frame:
        // ...

        if(keyboard->isKeyTapped(sf::Keyboard::Tab)) {
            setDebugInfo(!showDebugInfo());
        }
        if(keyboard->isKeyTapped(sf::Keyboard::O)) {
            setGlEnable(!glEnable());
        }
    }
};

int main(){
    ExampleScene exampleScene;

    exampleScene.create();
}