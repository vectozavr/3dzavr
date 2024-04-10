#include <iostream>

#include <Engine.h>
#include <io/Screen.h>
#include <utils/WorldEditor.h>
#include <components/props/Texture.h>
#include <animation/Animations.h>

class Test final : public Engine {
private:
    std::shared_ptr<WorldEditor> _worldEditor;

    void start() override {
        _worldEditor = std::make_shared<WorldEditor>(screen, world, camera);

        auto car = world->loadObject(ObjectTag("car_1"),
                FilePath("resources/obj/cars/car1/Car1.obj"));
        car->getComponent<TransformMatrix>()->rotate(Vec3D{0, Consts::PI, 0});
        car->getComponent<TransformMatrix>()->translate(Vec3D(0, 0, 10));

        Timeline::addAnimation<Wait>(0);
        Timeline::addAnimation<AttractToPoint>(car->getComponent<TransformMatrix>(), Vec3D(0), -3);
        Timeline::addAnimation<Wait>(0);
        Timeline::addAnimation<AttractToPoint>(car->getComponent<TransformMatrix>(), Vec3D(0), 3);
        Timeline::addAnimation<Wait>(0);

        auto carMesh = car->attached(ObjectTag("Car_car_mat_0"));
        if(carMesh) {
            Timeline::addAnimation<ShowUncreation>(carMesh->getComponent<TriangleMesh>(), 5);
            Timeline::addAnimation<Wait>(0);
            Timeline::addAnimation<ShowCreation>(carMesh->getComponent<TriangleMesh>(), 5);
            Timeline::addAnimation<Wait>(0);
            Timeline::addAnimation<Decompose>(carMesh->getComponent<TriangleMesh>(), 1);
            Timeline::addAnimation<Wait>(0);
            Timeline::addAnimation<Decompose>(carMesh->getComponent<TriangleMesh>(), -1);
        }

        Timeline::addAnimation<RotateRelativePoint>(car->getComponent<TransformMatrix>(), car->getComponent<TransformMatrix>()->position(), Vec3D{0, 2*Consts::PI, 0}, 3);
        Timeline::addAnimation<Wait>(0);
        Timeline::addAnimation<Rotate>(car->getComponent<TransformMatrix>(), Vec3D{0, 2*Consts::PI, 0}, 2);
        Timeline::addAnimation<Wait>(0);
        Timeline::addAnimation<RotateLeft>(car->getComponent<TransformMatrix>(), 2*Consts::PI, 2);
        Timeline::addAnimation<Wait>(0);
        Timeline::addAnimation<RotateLeftUpLookAt>(car->getComponent<TransformMatrix>(), Vec3D(0, 0, 2*Consts::PI), 2);
        Timeline::addAnimation<Scale>(car->getComponent<TransformMatrix>(), Vec3D(1.5, 1.3, 0.4), 2);
        Timeline::addAnimation<Wait>(0);
        Timeline::addAnimation<Translate>(car->getComponent<TransformMatrix>(), Vec3D(0, 0, 5));
        Timeline::addAnimation<Wait>(0);
        Timeline::addAnimation<TranslateToPoint>(car->getComponent<TransformMatrix>(), Vec3D(0, 0, 15));
        Timeline::addAnimation<Wait>(0);
        Timeline::addAnimation<Function>([car](){
            Log::log("Function test call");
            car->getComponent<TransformMatrix>()->translate(Vec3D(0, 0, 1));
        }, 3, 5);


        auto dirLight = std::make_shared<Object>(ObjectTag("Directional Light 1"));
        dirLight->addComponent<DirectionalLight>(Vec3D(1, -1, 1), Color::WHITE, 1.5);
        world->add(dirLight);
    };

    void update() override {
        screen->setTitle("3dzavr, " + std::to_string(Time::fps()) + "fps");

        _worldEditor->update();

        if(Keyboard::isKeyTapped(SDLK_TAB)) {
            setDebugInfo(!showDebugInfo());
        }
    };

public:
    Test() = default;
};

int main(int argc, char *argv[]) {
    Test test;
    test.create();
    return 0;
}