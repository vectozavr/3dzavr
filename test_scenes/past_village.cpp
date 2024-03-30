#include <iostream>

#include <Engine.h>
#include <io/Screen.h>
#include <utils/ResourceManager.h>
#include <utils/WorldEditor.h>
#include <objects/props/Texture.h>
#include <objects/lighting/PointLight.h>
#include <objects/lighting/SpotLight.h>
#include <animation/Animations.h>
#include <io/Keyboard.h>


class Test final : public Engine {
private:
    std::shared_ptr<WorldEditor> _worldEditor;

    void start() override {

        _worldEditor = std::make_shared<WorldEditor>(screen, world, camera);

        for (int i = 1; i <= 8; i++) {
            auto car = world->loadObject(
                    ObjectTag("car"+std::to_string(i)),
                    FilePath("resources/obj/cars/car"+std::to_string(i)+"/Car"+std::to_string(i)+".obj"));
            car->rotate(Vec3D{0, Consts::PI, 0});
            car->translate(Vec3D(-13.5 + 3*i, -4, 13));
        }

        //auto Dust = world->loadObject(ObjectTag("Dust"), FilePath("resources/obj/Dust/dust_map.obj"));

        auto PastVillage = world->loadObject(ObjectTag("PastVillage"), FilePath("resources/obj/PastVillage/PastVillage.obj"));
        PastVillage->scale(Vec3D{0.01, 0.01, 0.01});
        PastVillage->translate(Vec3D{0,-0.3,-14.5});
        camera->rotateUp(Consts::PI);

        auto skybox = std::make_shared<Group>(ObjectTag("skybox"));
        auto skyboxBack = ResourceManager::loadObject(ObjectTag("skybox_back"), FilePath("resources/obj/skybox/midday/skyboxShape.obj"));
        auto skyboxBigClouds = ResourceManager::loadObject(ObjectTag("skybox_big_clouds"), FilePath("resources/obj/skybox/midday/skybox1Shape.obj"));
        auto skyboxSmallClouds = ResourceManager::loadObject(ObjectTag("skybox_small_clouds"), FilePath("resources/obj/skybox/midday/skybox2Shape.obj"));
        if(skyboxBack && skyboxBigClouds && skyboxSmallClouds) {
            skybox->add(skyboxBack);
            skybox->add(skyboxBigClouds);
            skybox->add(skyboxSmallClouds);

            Timeline::addAnimation<ARotate>(AnimationListTag("cloud_animation"),
                                            skyboxBigClouds, Vec3D(0, 0.003, 0), 1,
                                            Animation::LoopOut::Continue);
            Timeline::addAnimation<ARotate>(AnimationListTag("cloud_animation"),
                                            skyboxSmallClouds, Vec3D(0, 0.005, 0), 1,
                                            Animation::LoopOut::Continue);
        }
        world->add(skybox);

        auto dir_light = std::make_shared<DirectionalLight>(
                ObjectTag("Directional Light 1"), Vec3D(1, -1, -1),
                Color::WHITE, 1.5);

        auto p_light = std::make_shared<PointLight>(
                ObjectTag("Point Light 1"), Vec3D(0, 0.5, -5),
                Color::LIGHT_YELLOW, 2);

        auto s_light = std::make_shared<SpotLight>(
                ObjectTag("Spot Light 1"),
                Vec3D(-7, 3, -12),
                Vec3D(0,-1,0));

        world->add(dir_light);
        world->add(p_light);
        world->add(s_light);

        auto lightCube = std::make_shared<TriangleMesh>(TriangleMesh::Cube(ObjectTag("LightCube"), 0.1));
        auto lightMaterial = std::make_shared<Material>(
                MaterialTag("Point Light Material"),
                nullptr,
                Color::LIGHT_YELLOW,
                Color(255, 200, 50, 100),
                Color(255, 200, 50, 100)
        );
        lightCube->setMaterial(lightMaterial);
        lightCube->translateToPoint(p_light->position());
        lightCube->attach(p_light);
        world->add(lightCube);
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