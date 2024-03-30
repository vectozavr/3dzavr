#include <iostream>

#include <Engine.h>
#include <io/Screen.h>
#include <utils/WorldEditor.h>
#include <objects/props/Texture.h>
#include <animation/Animations.h>

class Test final : public Engine {
private:
    std::shared_ptr<WorldEditor> _worldEditor;

    void start() override {
        _worldEditor = std::make_shared<WorldEditor>(screen, world, camera);

        auto car = world->loadObject(
                ObjectTag("car_1"),
                FilePath("resources/obj/cars/car1/Car1.obj"));
        car->rotate(Vec3D{0, Consts::PI, 0});
        car->translate(Vec3D(0, 0, 10));

        Timeline::addAnimation<AWait>(0);
        Timeline::addAnimation<AAttractToPoint>(car, Vec3D(0), -3);
        Timeline::addAnimation<AWait>(0);
        Timeline::addAnimation<AAttractToPoint>(car, Vec3D(0), 3);
        Timeline::addAnimation<AWait>(0);
        auto car_mesh = std::dynamic_pointer_cast<TriangleMesh>(car->object(ObjectTag("Car_car_mat_0")));
        if(car_mesh) {
            Timeline::addAnimation<AShowUncreation>(car_mesh, 5);
            Timeline::addAnimation<AWait>(0);
            Timeline::addAnimation<AShowCreation>(car_mesh, 5);
            Timeline::addAnimation<AWait>(0);
            Timeline::addAnimation<ADecompose>(car_mesh, 1);
            Timeline::addAnimation<AWait>(0);
            Timeline::addAnimation<ADecompose>(car_mesh, -1);
        }
        Timeline::addAnimation<ARotateRelativePoint>(car, car->position(), Vec3D{0, 2*Consts::PI, 0}, 3);
        Timeline::addAnimation<AWait>(0);
        Timeline::addAnimation<ARotate>(car, Vec3D{0, 2*Consts::PI, 0}, 2);
        Timeline::addAnimation<AWait>(0);
        Timeline::addAnimation<ARotateLeft>(car, 2*Consts::PI, 2);
        Timeline::addAnimation<AWait>(0);
        Timeline::addAnimation<ARotateLeftUpLookAt>(car, Vec3D(0, 0, 2*Consts::PI), 2);
        Timeline::addAnimation<AScale>(car, Vec3D(1.5, 1.3, 0.4), 2);
        Timeline::addAnimation<AWait>(0);
        Timeline::addAnimation<ATranslate>(car, Vec3D(0, 0, 5));
        Timeline::addAnimation<AWait>(0);
        Timeline::addAnimation<ATranslateToPoint>(car, Vec3D(0, 0, 15));
        Timeline::addAnimation<AWait>(0);
        Timeline::addAnimation<AFunction>([car](){
            Log::log("AFunction test call");
            car->translate(Vec3D(0, 0, 1));
        }, 3, 5);


        auto dir_light = std::make_shared<DirectionalLight>(
                ObjectTag("Directional Light 1"), Vec3D(1, -1, 1),
                Color::WHITE, 1.5);
        world->add(dir_light);
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