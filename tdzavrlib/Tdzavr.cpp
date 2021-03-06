//
// Created by Иван Ильин on 14.01.2021.
//

#include "Tdzavr.h"
#include "utils/Time.h"
#include <iostream>
#include "CameraMesh.h"
#include "ResourceManager.h"
#include "physics/Solver.h"

Tdzavr::Tdzavr() {
    screen = std::make_shared<Screen>();
    world = std::make_shared<World>();
    camera = std::make_shared<Camera>();
    external_camera = std::make_shared<Camera>();
}

void Tdzavr::create(int screenWidth, int screenHeight, const std::string &name, bool verticalSync, sf::Color background) {
    screen->open(screenWidth, screenHeight, name, verticalSync, background);

    Log::log("Tdzavr::create(): started 3dzavr (" + std::to_string(screenWidth) + " x " + std::to_string(screenHeight) + ") with name '" + name + "'.");
    Time::update();

    start();
    camera->init(screenWidth, screenHeight);
    external_camera->init(screenWidth, screenHeight,90.0, 0.1, 1000.0);
    external_camera->makeExternal();
    screen->getMouseDisplacement(); // We do it to set mouse position in the center (see how getMouseDisplacement() works)

    while (screen->isOpen()) {
        screen->clear();

        Time::update();
        screen->keyboardControl();
        update(Time::deltaTime());

        /* Project all mesh
         * Here we project all tris for each mesh from world._objects.
         * When we call camera.project(m.second),
         */

        // sometimes we dont need to update physics world
        // (for example in menu or while pause)
        // hence we can set 'b_updateWorld' equal to false in setUpdateWorld(bool):
        if(b_updateWorld) {
            camera->record();
            for (auto &m : world->objects()) {
                m.second->a_update();
                camera->project(*m.second, screen->mode());

                m.second->updatePhysicsState();
                // isCollision detection:
                if (m.second->isCollision()) {
                    m.second->setInCollision(false);
                    for (auto &obj : world->objects()) {
                        if (obj.first != m.first && obj.second->isCollider()) {
                            std::pair<bool, Simplex> gjk = m.second->checkGJKCollision(obj.second);
                            if (gjk.first) {
                                CollisionPoint epa = m.second->EPA(gjk.second, obj.second);
                                Solver::solveCollision(m.second, obj.second, epa);
                            }
                        }
                    }
                }

            }

            // draw projected mesh
            if (cameraMode == LocalCamera) {
                // Draw from camera view
                for (auto &t : camera->sorted())
                    screen->triangle(t);
            } else {
                // Draw from the perspective of external observer
                external_camera->record();
                Mesh tracedTriangles(camera->tracedTriangles());
                CameraMesh cameraMesh(*camera);


                external_camera->project(tracedTriangles, screen->mode());
                external_camera->project(cameraMesh, screen->mode());

                for (auto &t : external_camera->sorted())
                    screen->triangle(t);
            }

            camera->a_update();
            external_camera->a_update();

            triPerSec = camera->buffSize() * Time::fps();

            if (b_debugText) {
                Point4D cameraPosition{};
                double cameraY;
                if (cameraMode == CameraMode::LocalCamera)
                    cameraPosition = camera->eye();
                else
                    cameraPosition = external_camera->eye();

                screen->debugText(name + "\n\n X: " +
                                 std::to_string((cameraPosition.x)) + "\n Y: " +
                                 std::to_string((cameraPosition.y)) + "\n Z: " +
                                 std::to_string((cameraPosition.z)) + "\n\n" +
                                 std::to_string(screen->width()) + "x" +
                                 std::to_string(screen->height()) + "\n" +
                                 std::to_string(Time::fps()) +
                                 " fps \n" + std::to_string((int) triPerSec) + " tris/s");
            }
        }

        gui();
        screen->display();
    }
    exit();
}

void Tdzavr::exit() {
    if(screen->isOpen()) {
        screen->close();
        if(screen->isRender())
            screen->setRender(false);
    }
    ResourceManager::unloadAllResources();
    Log::log("Tdzavr::exit(): exit 3dzavr (" + std::to_string(screen->width()) + " x " + std::to_string(screen->height()) + ") with name '" + screen->title() + "'.");
}

void Tdzavr::setCameraMode(Tdzavr::CameraMode mode) {
    cameraMode = mode;
    if(mode == ExternalObserver)
        camera->setTrace(true);
    else
        camera->setTrace(false);
}
