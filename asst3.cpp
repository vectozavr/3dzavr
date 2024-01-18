//
// Created by Ivan Ilin on 26/10/2023.
//

#include <iostream>
#include <cmath>

#include <Engine.h>
#include <io/Screen.h>
#include "objects/geometry/Sphere.h"
#include <utils/ObjectController.h>

#include <animation/Animations.h>

#include <objects/geometry/Texture.h>

class Test final : public Engine {
private:
    std::shared_ptr<ObjectController> cameraController = nullptr;
    std::shared_ptr<ObjectController> objController = nullptr;
    std::shared_ptr<Object> selectedObject = nullptr;
    bool objSelected = false;

    std::shared_ptr<Mesh> house = nullptr;

    void start() override {


        std::shared_ptr<Sphere> sphere_1 = std::make_shared<Sphere>(0.7,
                                                                    Vec3D(-1.5, -0.3, 4),
                                                                    ObjectNameTag("sphere_1"),
                                                                    Consts::WHITE_COLORS[0]);
        //world->add(sphere_1);

        std::shared_ptr<Sphere> sphere_2 = std::make_shared<Sphere>(1,
                                                                    Vec3D(0.5, 0, 3),
                                                                    ObjectNameTag("sphere_2"),
                                                                    Consts::WHITE_COLORS[2]);
        //world->add(sphere_2);

        std::shared_ptr<Plane> plane = std::make_shared<Plane>(Vec3D(0, 1, 0),
                                                               Vec3D(0,-1,0),
                                                               ObjectNameTag("plane"),
                                                               Consts::WHITE*0.5);

        //world->add(plane);

        std::shared_ptr<DirectionalLight> light = std::make_shared<DirectionalLight>(ObjectNameTag("Directional Light Source 1"),
                                                                                     Vec3D(1, 1, -1),
                                                                                     Consts::WHITE);

        //world->add(light);

        cameraController = std::make_shared<ObjectController>(camera);


        //house = world->loadMesh(ObjectNameTag("house"), "obj/house/mesh.obj", "obj/house/color_r1.png");
        house = world->loadMesh(ObjectNameTag("building"), "obj/building/mesh.obj", "obj/building/color_r1.png");
        house->translate(Vec3D(0, -3, 10));

        auto texture = std::make_shared<Texture>("obj/prototype-textures/png/concrete.png");

        for(int i = -5; i < 5; i++) {
            for(int j = -5; j < 5; j++) {
                auto surface = std::make_shared<Mesh>(Mesh::Surface(
                        ObjectNameTag("surface_" + std::to_string(i) + "_" + std::to_string(j)),
                        10, 10, texture));
                surface->translate(Vec3D(10*i, -3, 10*j));
                world->add(surface);
            }
        }

        //camera->transform(Matrix4x4(
        //        std::array<std::array<double, 4>, 4>({std::array<double, 4>({0.611374, 0.0427117, -0.790188, 5.6313}),
        //                                                    std::array<double, 4>({-4.77049e-18, 0.998542, 0.0539738, -1.08648}),
        //                                              std::array<double, 4>({0.791341, -0.0329982, 0.610483, 1.58696}),
        //                                              std::array<double, 4>({0, 0, 0, 1})})));

    };

    void update() override {

        // Testing drawing Line:
        /*
        for(int i = 0; i < 16; i++) {
            screen->drawLine(Vec2D(50, 50), Vec2D(50, 50) +
                                            Vec2D(cos(2.0*Consts::PI*i/16.0), sin(2.0*Consts::PI*i/16.0))*30, Consts::WHITE_COLORS[i%5], 1);
        }
        */

        //world->object(ObjectNameTag("building"))->rotateUp(Time::deltaTime()/2);

        screen->setTitle("3dzavr, " + std::to_string(Time::fps()) + "fps");
        if(objSelected) {
            objController->update();
        } else {
            cameraController->update();
        }

        // select object:
        if (Keyboard::isKeyTapped(SDLK_o)) {
            auto rayCast = world->rayCast(camera->position(), camera->position() + camera->lookAt(), "plane");

            if(rayCast.intersected) {
                objSelected = true;
                selectedObject = rayCast.obj;
                objController = std::make_shared<ObjectController>(selectedObject);
                Log::log("Object " + rayCast.objectName.str() + " selected.");
            }

            std::cout << camera->model();
        }

        // deselect object:
        if (Keyboard::isKeyTapped(SDLK_p)) {
            objSelected = false;
        }

        // object scale x:
        if(Keyboard::isKeyPressed(SDLK_UP)) {
            selectedObject->scaleInside(Vec3D(1 + Time::deltaTime(), 1, 1));
        }
        // object scale y:
        if(Keyboard::isKeyPressed(SDLK_DOWN)) {
            selectedObject->scaleInside(Vec3D(1, 1 + Time::deltaTime(), 1));
        }
        // object scale z:
        if(Keyboard::isKeyPressed(SDLK_LEFT)) {
            selectedObject->scaleInside(Vec3D(1, 1, 1 + Time::deltaTime()));
        }

        // undo transformations
        if(Keyboard::isKeyPressed(SDLK_u)) {
            selectedObject->transform(selectedObject->invModel());
        }

        // print some debug
        if(Keyboard::isKeyTapped(SDLK_i)) {
            Timeline::addAnimation<ADecompose>(house, 5, 5);
            Timeline::addAnimation<ARotate>(house, Vec3D(0, 5, 0), 10);
            Timeline::addAnimation<AWait>();
            Timeline::addAnimation<ADecompose>(house, -5, 5);
        }

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
}