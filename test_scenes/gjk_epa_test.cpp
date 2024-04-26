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
        cube1->addComponent<RigidObject>()->setCollision(false);

        auto boundFn = std::bind(&Test::collisionHandler, this,
                                 std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
        cube1->getComponent<RigidObject>()->setCollisionCallBack(boundFn);

        cube1->addComponent<TriangleMesh>(TriangleMesh::Cube())->setMaterial(transparentYellow);

        //cube1->getComponent<TransformMatrix>()->scale(Vec3D(0.5, 10, 10));
        cube1->getComponent<TransformMatrix>()->translate(Vec3D(2, 0, 0));
        //cube1->getComponent<TransformMatrix>()->translate(Vec3D(-0.1, 0.45, 3.4));

        world->add(cube1);

        auto cube2 = std::make_shared<Object>(ObjectTag("cube_2"));
        cube2->addComponent<RigidObject>()->setCollision(true);
        cube2->addComponent<TriangleMesh>(TriangleMesh::Cube())->setMaterial(transparentYellow);

        cube2->getComponent<TransformMatrix>()->translate(Vec3D(0, 0, 0));
        world->add(cube2);

        auto dirLight = std::make_shared<Object>(ObjectTag("Directional Light 1"));
        dirLight->addComponent<DirectionalLight>(Vec3D(1, -1, 1), Color::WHITE, 1.5);
        world->add(dirLight);

        //cube2->getComponent<RigidObject>()->setAcceleration(Vec3D(1, 0, 0));

        camera->transformMatrix()->translateToPoint(Vec3D(3.3, 2.5, 0.2));
        camera->transformMatrix()->rotateRelativeItself(Vec3D(0, -1, 0));
        camera->transformMatrix()->rotateLeft(0.4);
    };

    void update() override {
        screen->setTitle("3dzavr, " + std::to_string(Time::fps()) + "fps");

        _worldEditor->update();

        if(Keyboard::isKeyTapped(SDLK_TAB)) {
            setDebugInfo(!showDebugInfo());
        }
    };

    void collisionHandler(const EPA::CollisionPoint& point,
                          std::shared_ptr<RigidObject> obj1,
                          std::shared_ptr<RigidObject> obj2) {

        auto collisionInfo = std::make_shared<Group>(ObjectTag("collision_info"));
        // Support vectors on cubes
        int i = 0;
        for(const auto& edge: {point.edge1, point.edge2, point.edge3}) {
            auto vec = Object(ObjectTag("cube_v" + std::to_string(i++)));
            vec.addComponent<TriangleMesh>(TriangleMesh::ArrowTo(edge.p1, edge.p2, 0.01));
            collisionInfo->add(vec);
        }

        auto redMaterial = std::make_shared<Material>(MaterialTag("red"),
                                                      nullptr, Color::RED);
        auto greenMaterial = std::make_shared<Material>(MaterialTag("green"),
                                                        nullptr, Color::GREEN);

        // Support points
        i = 0;
        for(const auto& p : {point.edge1, point.edge2, point.edge3}) {
            auto cubeEdge1 = Object(ObjectTag("cube_edge_" + std::to_string(i++)));
            cubeEdge1.addComponent<TriangleMesh>(TriangleMesh::Cube(0.1))->setMaterial(redMaterial);
            cubeEdge1.getComponent<TransformMatrix>()->translateToPoint(p.p1);
            collisionInfo->add(cubeEdge1);

            auto cubeEdge2 = Object(ObjectTag("cube_edge_" + std::to_string(i++)));
            cubeEdge2.addComponent<TriangleMesh>(TriangleMesh::Cube(0.1))->setMaterial(greenMaterial);
            cubeEdge2.getComponent<TransformMatrix>()->translateToPoint(p.p2);
            collisionInfo->add(cubeEdge2);

            auto cubeEdgeEPA = Object(ObjectTag("cube_edge_EPA_" + std::to_string(i++)));
            cubeEdgeEPA.addComponent<TriangleMesh>(TriangleMesh::Cube(0.1))->setMaterial(redMaterial);
            cubeEdgeEPA.getComponent<TransformMatrix>()->translateToPoint(p.support);
            collisionInfo->add(cubeEdgeEPA);
        }

        // EPA polytope edges
        i = 0;
        for(const auto& edge: point.polytope) {
            auto edgeObj = Object(ObjectTag("edge_EPA_" + std::to_string(i++)));
            edgeObj.addComponent<TriangleMesh>(TriangleMesh::ArrowTo(Vec3D(0), edge.support, 0.01));
            collisionInfo->add(edgeObj);
        }

        auto blueMaterial = std::make_shared<Material>(MaterialTag("blue"),
                                                       nullptr, Color::BLUE);


        i = 0;
        for(const auto &p : point.collisionPlane.points) {
            auto collisionP = Object(ObjectTag("collisionP" + std::to_string(i++)));
            collisionP.addComponent<TriangleMesh>(TriangleMesh::Cube(0.1))->setMaterial(blueMaterial);
            collisionP.getComponent<TransformMatrix>()->translateToPoint(p);
            collisionInfo->add(collisionP);
        }
        world->replace(collisionInfo);
        std::cout << "Points: " << point.collisionPlane.points.size() << std::endl;
    }

public:
    Test() = default;
};

int main(int argc, char *argv[]) {
    Test test;
    test.create();

    /*
    EPA::PlanePoints points;
    points.points.emplace_back(0, 0);
    points.points.emplace_back(0, 1);
    points.points.emplace_back(1, 1);
    points.points.emplace_back(1, 0);

    Vec3D origin(0.5, 0.5);

    for(int i = 0; i < points.points.size(); i++) {
        Vec3D oa = points.points[i] - origin;
        Vec3D ab = points.points[(i+1)%points.points.size()] - points.points[i];
        Vec3D abNormalized = ab.normalized();


        Vec3D lineNorm = oa - abNormalized*oa.dot(abNormalized);
        lineNorm = lineNorm.normalized();

        points.normals.emplace_back(lineNorm);
    }


    Vec3D a(0.5, 0.5);
    Vec3D b(2, 0.5);

    auto res = points.intersectionWithLine(a, b);
    */
    return 0;
}
