//
// Created by Ivan Ilin on 26/10/2023.
//

#include <iostream>

#include <Engine.h>
#include <io/Screen.h>
#include "objects/geometry/Sphere.h"
#include <utils/ObjectController.h>


#include "objects/props/Texture.h"

class Test final : public Engine {
private:
    std::shared_ptr<ObjectController> cameraController = nullptr;
    std::shared_ptr<ObjectController> objController = nullptr;
    std::shared_ptr<Object> selectedObject = nullptr;
    bool objSelected = false;

    std::shared_ptr<Group> obj = nullptr;

    void start() override {
        cameraController = std::make_shared<ObjectController>(camera);

        obj = world->loadObject(ObjectTag("car1"), FilePath("resources/obj/car/Car.obj"));
        obj->translate(Vec3D(0, -3, 10));
    };

    void update() override {
        screen->setTitle("3dzavr, " + std::to_string(Time::fps()) + "fps");

        screen->drawStrokeRectangle(Consts::STANDARD_SCREEN_WIDTH/2, Consts::STANDARD_SCREEN_HEIGHT/2, 3, 3, Consts::BLACK);

        if(objSelected) {
            objController->update();
        } else {
            obj->rotateRelativePoint(obj->position(), Vec3D{0, 0.5*Time::deltaTime(), 0});

            cameraController->update();
        }

        // select object:
        if (Keyboard::isKeyTapped(SDLK_o)) {
            auto rayCast = world->rayCast(camera->position(), camera->position() + camera->lookAt());

            if(rayCast.intersected) {
                objSelected = true;
                selectedObject = rayCast.obj;
                objController = std::make_shared<ObjectController>(selectedObject);
                Log::log("Object " + rayCast.objectName.str() + " selected.");
            }
        }

        // deselect object:
        if (Keyboard::isKeyTapped(SDLK_p)) {
            objSelected = false;
            selectedObject.reset();
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