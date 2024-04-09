#include <iostream>

#include <Engine.h>
#include <io/Screen.h>
#include <utils/ResourceManager.h>
#include <utils/WorldEditor.h>
#include <components/props/Texture.h>
#include <components/lighting/PointLight.h>
#include <components/lighting/SpotLight.h>
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
            car->getComponent<TransformMatrix>()->rotate(Vec3D{0, Consts::PI, 0});
            car->getComponent<TransformMatrix>()->translate(Vec3D(-13.5 + 3*i, -4, 13));
        }

        auto PastVillage = world->loadObject(ObjectTag("PastVillage"),
                                             FilePath("resources/obj/PastVillage/PastVillage.obj"));
        PastVillage->getComponent<TransformMatrix>()->scale(Vec3D{0.01, 0.01, 0.01});
        PastVillage->getComponent<TransformMatrix>()->translate(Vec3D{0,-0.3,-14.5});
        camera->transformMatrix()->rotateUp(Consts::PI);

        auto skybox = std::make_shared<Group>(ObjectTag("skybox"));
        auto skyboxBack = ResourceManager::loadTriangleMesh(ObjectTag("skybox_back"),
                                                            FilePath("resources/obj/skybox/midday/skyboxShape.obj"));
        auto skyboxBigClouds = ResourceManager::loadTriangleMesh(ObjectTag("skybox_big_clouds"),
                                                                 FilePath("resources/obj/skybox/midday/skybox1Shape.obj"));
        auto skyboxSmallClouds = ResourceManager::loadTriangleMesh(ObjectTag("skybox_small_clouds"),
                                                                   FilePath("resources/obj/skybox/midday/skybox2Shape.obj"));
        if(skyboxBack && skyboxBigClouds && skyboxSmallClouds) {
            skybox->add(skyboxBack);
            skybox->add(skyboxBigClouds);
            skybox->add(skyboxSmallClouds);

            Timeline::addAnimation<ARotate>(AnimationListTag("cloud_animation"),
                                            skyboxBigClouds->getComponent<TransformMatrix>(), Vec3D(0, 0.003, 0), 1,
                                            Animation::LoopOut::Continue);
            Timeline::addAnimation<ARotate>(AnimationListTag("cloud_animation"),
                                            skyboxSmallClouds->getComponent<TransformMatrix>(), Vec3D(0, 0.005, 0), 1,
                                            Animation::LoopOut::Continue);
        }
        world->add(skybox);

        auto directionalLight = std::make_shared<Object>(ObjectTag("Dir Light 1"));
        directionalLight->addComponent<DirectionalLight>(Vec3D(1, -1, -1),Color::WHITE, 1.5);

        auto pointLight = std::make_shared<Object>(ObjectTag("Point Light 1"));
        pointLight->addComponent<PointLight>(Vec3D(0, 0.5, -5),Color::LIGHT_YELLOW, 2);
        auto lightCubeTriangleMesh = pointLight->addComponent<TriangleMesh>(TriangleMesh::Cube(0.1));
        auto lightMaterial = std::make_shared<Material>(
                MaterialTag("Point Light Material"),
                nullptr,
                Color::LIGHT_YELLOW,
                Color(255, 200, 50, 100),
                Color(255, 200, 50, 100)
        );
        lightCubeTriangleMesh->setMaterial(lightMaterial);
        pointLight->getComponent<TransformMatrix>()->translateToPoint(Vec3D(0, 0.5, -5));

        auto spotLight = std::make_shared<Object>(ObjectTag("Spot Light 1"));
        spotLight->addComponent<SpotLight>(Vec3D(-7, 3, -12), Vec3D(0,-1,0));

        world->add(directionalLight);
        world->add(pointLight);
        world->add(spotLight);
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