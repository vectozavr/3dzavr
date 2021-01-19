//
// Created by Иван Ильин on 14.01.2021.
//

#include "Tdzavr.h"
#include "Time.h"
#include <iostream>
#include "CameraMesh.h"

void Tdzavr::create(int screenWidth, int screenHeight, const std::string &name, bool verticalSync, sf::Color background) {
    screen.open(screenWidth, screenHeight, name, verticalSync, background);

    Log::log("Tdzavr::create(): started 3dzavr (" + std::to_string(screenWidth) + " x " + std::to_string(screenHeight) + ") with name '" + name + "'.");
    Time::update();

    start();
    camera.init(screenWidth, screenHeight);
    external_camera.init(screenWidth, screenHeight,90.0, 0.1, 1000.0);
    external_camera.makeExternal();
    screen.getMouseDisplacement(); // We do it to set mouse position in the center (see how getMouseDisplacement() works)

    while (screen.isOpen()) {
        screen.clear();

        Time::update();
        update(Time::deltaTime());

        /* Project all mesh
         * Here we project all triangles for each mesh from world.objects.
         * When we call camera.project(m.second),
         */
        camera.record();
        for(auto& m : world.objects)
            camera.project(m.second, screen.mode());

        // draw projected mesh
        if(cameraMode == LocalCamera) {
            // Draw from camera view
            for (auto &t : camera.sorted())
                screen.triangle(t);
        }
        else {
            // Draw from the perspective of external observer
            external_camera.record();
            Mesh tracedTriangles(camera.tracedTrianglesSorted());
            CameraMesh cameraMesh(camera);


            external_camera.project(tracedTriangles, screen.mode());
            external_camera.project(cameraMesh, screen.mode());

            for (auto &t : external_camera.sorted())
                screen.triangle(t);
        }

        triPerSec = camera.buffSize() * Time::fps();

        if(cameraMode == CameraMode::LocalCamera)
            screen.debugText(name + "\n\n X: " + std::to_string((camera.eye().x)) + "\n Y: " + std::to_string((camera.eye().y)) + "\n Z: " + std::to_string((camera.eye().z)) + "\n\n" + std::to_string(Time::fps()) + " fps \n" + std::to_string((int)triPerSec) + " triangles/s");
        else
            screen.debugText(name + "\n\n X: " + std::to_string((external_camera.eye().x)) + "\n Y: " + std::to_string((external_camera.eye().y)) + "\n Z: " + std::to_string((external_camera.eye().z)) + "\n\n" + std::to_string(Time::fps()) + " fps \n" + std::to_string((int)triPerSec) + " triangles/s");


        screen.display();
    }
    exit();
}

void Tdzavr::exit() {
    if(screen.isOpen())
        screen.close();
    Log::log("Tdzavr::exit(): exit 3dzavr (" + std::to_string(screen.width()) + " x " + std::to_string(screen.height()) + ") with name '" + screen.title() + "'.");
}

void Tdzavr::setCameraMode(Tdzavr::CameraMode mode) {
    cameraMode = mode;
    if(mode == ExternalObserver)
        camera.setTrace(true);
    else
        camera.setTrace(false);
}
