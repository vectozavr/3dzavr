#include <iostream>

#include <Engine.h>
#include <io/Screen.h>
#include <utils/WorldEditor.h>
#include <components/lighting/PointLight.h>
#include <utils/ResourceManager.h>

class Test final : public Engine {
private:
    std::shared_ptr<WorldEditor> _worldEditor;
    int _cubes = 0;

    void start() override {
        _worldEditor = std::make_shared<WorldEditor>(screen, world, camera);
        _worldEditor->setEnableLighting(false);

        auto cube1 = std::make_shared<Object>(ObjectTag("cube_1"));
        cube1->addComponent<RigidObject>()->setCollision(true);

        auto r = ResourceManager::loadTriangleMesh(ObjectTag("thrown_cube" + std::to_string(_cubes++)),
                                                   FilePath("resources/obj/box/paper_1/paper.obj"));
        auto paperCube = r->find(ObjectTag("Cardboard_Box_cardboxes_128_cardboxes_128_0"));
        //cube1->addComponent<TriangleMesh>(*paperCube->getComponent<TriangleMesh>());
        cube1->addComponent<TriangleMesh>(TriangleMesh::Cube());


        //cube1->getComponent<TransformMatrix>()->translate(Vec3D(-0.1, 0.45, 3.4));
        cube1->getComponent<TransformMatrix>()->translate(Vec3D(2, 0.0001, 4.0001));

        //cube1->getComponent<RigidObject>()->setVelocity(Vec3D(-1,0,0));
        //cube1->getComponent<RigidObject>()->setAcceleration(Vec3D(0,-1,0));
        auto boundFn = std::bind(&Test::collisionHandler, this,
                                 std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
        cube1->getComponent<RigidObject>()->setCollisionCallBack(boundFn);
        cube1->getComponent<RigidObject>()->setMass(1000000);

        world->add(cube1);

        auto cube2 = std::make_shared<Object>(ObjectTag("cube_2"));
        cube2->addComponent<RigidObject>()->setCollision(true);

        //cube2->addComponent<TriangleMesh>(*paperCube->getComponent<TriangleMesh>());
        cube2->addComponent<TriangleMesh>(TriangleMesh::Cube());


        cube2->getComponent<TransformMatrix>()->translate(Vec3D(-2, 0, 4));
        world->add(cube2);

        cube2->getComponent<RigidObject>()->setVelocity(Vec3D(1,0,0));


        auto ground = std::make_shared<Object>(ObjectTag("ground"));
        ground->addComponent<RigidObject>();
        ground->addComponent<TriangleMesh>(TriangleMesh::Cube());
        ground->getComponent<TransformMatrix>()->scale(Vec3D(30,1,30));
        ground->getComponent<TransformMatrix>()->translate(Vec3D(0, -5, 4));
        ground->getComponent<TriangleMesh>()->getMaterial()->setTexture(std::make_shared<Texture>(FilePath("resources/obj/PastVillage/mr_032f_yane.png")));
        ground->getComponent<RigidObject>()->setMass(1000000);


        world->add(ground);

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

        if(Keyboard::isKeyTapped(SDLK_c)) {
            auto cube = std::make_shared<Object>(ObjectTag("thrown_cube" + std::to_string(_cubes++)));
            cube->addComponent<RigidObject>()->setCollision(true);

            auto r = ResourceManager::loadTriangleMesh(ObjectTag("thrown_cube" + std::to_string(_cubes++)),
                                              FilePath("resources/obj/box/paper_1/paper.obj"));

            auto paperCube = r->find(ObjectTag("Cardboard_Box_cardboxes_128_cardboxes_128_0"));

            cube->addComponent<TriangleMesh>(*paperCube->getComponent<TriangleMesh>());

            Vec3D camPos = camera->transformMatrix()->fullPosition();
            Vec3D camLookAt = camera->transformMatrix()->lookAt();


            cube->getComponent<TransformMatrix>()->translateToPoint(camera->transformMatrix()->fullPosition());

            cube->getComponent<RigidObject>()->setVelocity(camera->transformMatrix()->lookAt());
            cube->getComponent<RigidObject>()->setAcceleration(Vec3D(0,-10,0));

            world->add(cube);
        }
    };

    void collisionHandler(const EPA::CollisionPoint& point,
                          std::shared_ptr<RigidObject> obj1,
                          std::shared_ptr<RigidObject> obj2) {

        auto blueMaterial = std::make_shared<Material>(MaterialTag("blue"),
                                                       nullptr, Color::BLUE);

        auto collisionP1 = Object(ObjectTag("collisionP1"));
        collisionP1.addComponent<TriangleMesh>(TriangleMesh::Cube(0.1))->setMaterial(blueMaterial);
        collisionP1.getComponent<TransformMatrix>()->translateToPoint(point.collisionPlane.origin);
        world->replace(collisionP1);
    }

public:
    Test() = default;
};

int main(int argc, char *argv[]) {
    Test test;
    test.create();
    return 0;
}
