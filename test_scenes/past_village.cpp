#include <iostream>

#include <Engine.h>
#include <io/Screen.h>
#include <utils/ObjectController.h>
#include <objects/props/Texture.h>
#include <objects/lighting/PointLight.h>
#include <animation/Animations.h>

class Test final : public Engine {
private:
    std::shared_ptr<ObjectController> cameraController = nullptr;
    std::shared_ptr<ObjectController> objController = nullptr;
    std::shared_ptr<Object> selectedObject = nullptr;
    std::shared_ptr<Mesh> redCube = nullptr;
    bool objSelected = false;
    bool objInFocus = false;
    bool isControllerActive = true;
    bool isRecording = false;

    void start() override {
        cameraController = std::make_shared<ObjectController>(camera);

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

        auto skybox = world->loadObject(ObjectTag("skybox"), FilePath("resources/obj/skybox/midday/skyboxShape.obj"));
        //auto skybox = world->loadObject(ObjectTag("skybox"), FilePath("resources/obj/skybox/sunset/skybox.obj"));

        //skybox->scale(Vec3D{0.1, 0.1, 0.1});

        // For debug
        redCube = std::make_shared<Mesh>(Mesh::Cube(ObjectTag("RedCube"), 0.1));
        redCube->setVisible(objInFocus);
        //world->add(redCube);

        auto orangeGlass = std::make_shared<Material>(
                MaterialTag("Transparent orange glass"),
                nullptr,
                Color(255, 200, 50, 100),
                Color(255, 200, 50, 100),
                Color(255, 200, 50, 100)
                );
        redCube->setMaterial(orangeGlass);

        //auto greenwood = world->loadObject(ObjectTag("greenwood"),
        //                                   FilePath("resources/obj/cars/greenwood/greenwood.obj"));
        //greenwood->translate(Vec3D{0, -0.3, -10});
        //greenwood->scale(Vec3D(0.3, 0.3, 0.3));
        //greenwood->rotateUp(-3*Consts::PI/4);

        auto dir_light = std::make_shared<DirectionalLight>(
                ObjectTag("Directional Light 1"), Vec3D(1, -1, -1),
                Color::WHITE, 1.5);

        auto p_light = std::make_shared<PointLight>(
                ObjectTag("Point Light 1"), Vec3D(0, 0.5, -5),
                Color::LIGHT_YELLOW, 2);

        world->add(dir_light);
        world->add(p_light);

        auto lightCube = std::make_shared<Mesh>(Mesh::Cube(ObjectTag("LightCube"), 0.1));
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

        screen->drawStrokeRectangle(Consts::STANDARD_SCREEN_WIDTH/2-1, Consts::STANDARD_SCREEN_HEIGHT/2-7, 1, 14, Color::BLACK);
        screen->drawStrokeRectangle(Consts::STANDARD_SCREEN_WIDTH/2-7, Consts::STANDARD_SCREEN_HEIGHT/2-1, 14, 1, Color::BLACK);

        if(isControllerActive) {
            if(objSelected) {
                objController->update();
            } else {
                cameraController->update();
            }
        }

        auto rayCast = world->rayCast(camera->position(), camera->position() + camera->lookAt(), {redCube->name()});

        objInFocus = rayCast.intersected;
        redCube->setVisible(objInFocus);
        if(objInFocus) {
            redCube->translateToPoint(rayCast.pointOfIntersection);
        }

        // select object:
        if (Keyboard::isKeyTapped(SDLK_o) || Mouse::isButtonTapped(SDL_BUTTON_LEFT)) {
            if(objInFocus) {
                objSelected = true;
                selectedObject = rayCast.obj;
                objController = std::make_shared<ObjectController>(selectedObject);
                Log::log("Object " + rayCast.obj->name().str() + " selected.");
                //Timeline::addAnimation<ATranslateToPoint>(camera, selectedObject->position() - (selectedObject->position() - camera->position()).normalized());
            }
        }

        // deselect object:
        if (Keyboard::isKeyTapped(SDLK_p)) {
            objSelected = false;
            selectedObject.reset();
        }

        if(Keyboard::isKeyTapped(SDLK_F1)) {
            screen->makeScreenShot().save2png(FilePath("screenshot_" + Time::getLocalTimeInfo("%F_%H-%M-%S") + ".png"));
        }

        if(Keyboard::isKeyTapped(SDLK_F2)) {
            if(!isRecording) {
                screen->startRender();
            } else{
                screen->stopRender();
            }
            isRecording = !isRecording;
        }

        if(Keyboard::isKeyTapped(SDLK_q)) {
            isControllerActive = !isControllerActive;
        }

        if (selectedObject && isControllerActive) {
            // object scale x:
            if (Keyboard::isKeyPressed(SDLK_UP)) {
                selectedObject->scaleInside(Vec3D(1 + Time::deltaTime(), 1, 1));
            }
            // object scale y:
            if (Keyboard::isKeyPressed(SDLK_DOWN)) {
                selectedObject->scaleInside(Vec3D(1, 1 + Time::deltaTime(), 1));
            }
            // object scale z:
            if (Keyboard::isKeyPressed(SDLK_LEFT)) {
                selectedObject->scaleInside(Vec3D(1, 1, 1 + Time::deltaTime()));
            }

            // undo transformations
            if (Keyboard::isKeyPressed(SDLK_u)) {
                selectedObject->transform(selectedObject->invModel());
            }

            // delete object
            if (Keyboard::isKeyPressed(SDLK_DELETE)) {
                world->remove(selectedObject->name());
                objSelected = false;
                selectedObject.reset();
            }
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
    return 0;
}