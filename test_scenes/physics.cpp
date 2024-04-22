#include <iostream>

#include <Engine.h>
#include <io/Screen.h>
#include <utils/WorldEditor.h>
#include <components/lighting/PointLight.h>

class Test final : public Engine {
private:
    std::shared_ptr<WorldEditor> _worldEditor;

    void start() override {
        _worldEditor = std::make_shared<WorldEditor>(screen, world, camera);

        auto transparentYellow = std::make_shared<Material>(MaterialTag("yellow"),
                                                      nullptr, Color(255, 200, 170, 100));

        auto cube1 = std::make_shared<Object>(ObjectTag("cube_1"));
        cube1->addComponent<RigidObject>()->setCollision(true);
        cube1->addComponent<TriangleMesh>(TriangleMesh::Cube())->setMaterial(transparentYellow);

        //cube1->getComponent<TransformMatrix>()->translate(Vec3D(-0.1, 0.45, 3.4));
        cube1->getComponent<TransformMatrix>()->translate(Vec3D(1, 0, 3));

        world->add(cube1);

        auto cube2 = std::make_shared<Object>(ObjectTag("cube_2"));
        cube2->addComponent<RigidObject>();
        cube2->addComponent<TriangleMesh>(TriangleMesh::Cube())->setMaterial(transparentYellow);

        cube2->getComponent<TransformMatrix>()->translate(Vec3D(-1, 0, 3));
        world->add(cube2);

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
