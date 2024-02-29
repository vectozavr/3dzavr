//
// Created by Иван Ильин on 14.01.2021.
//

#include <iostream>

#include <Engine.h>
#include <utils/Time.h>
#include <utils/ResourceManager.h>
#include <animation/Timeline.h>
#include <io/Keyboard.h>
#include <io/Mouse.h>
#include <cmath>

Engine::Engine() {
    Time::init();
    Timeline::init();
    Keyboard::init();
    Mouse::init();
    ResourceManager::init();
}

void Engine::projectAndDrawGroup(std::shared_ptr<Group> group) const {

    /*
     * TODO: we spend too much time on iteration through all of the objects in the scene.
     * Maybe we can exclude some objects on the early stage based on the space separation.
     */

    for(const auto& [objTag, obj] : *group) {
        std::shared_ptr<Mesh> subMesh = std::dynamic_pointer_cast<Mesh>(obj);
        if(subMesh) {
            // project triangles to the camera plane
            Time::startTimer("d projections");
            auto projected = camera->project(subMesh);
            Time::pauseTimer("d projections");

            // draw projected triangles
            Time::startTimer("d rasterization");
            for (auto &t : projected) {
                screen->drawTriangle(*t.first, t.second.get());
            }
            Time::pauseTimer("d rasterization");
            continue;
        }
        std::shared_ptr<Group> subGroup = std::dynamic_pointer_cast<Group>(obj);
        if(subGroup) {
            // We need to recursively continue to draw subgroup
            projectAndDrawGroup(subGroup);
        }
    }
}

void Engine::create(uint16_t screenWidth, uint16_t screenHeight, const Color& background) {

    screen->open(screenWidth, screenHeight, background);

    Log::log("Engine::create(): started 3dzavr. Screen size: (" + std::to_string(screenWidth) + "x" + std::to_string(screenHeight) + ")");
    Time::update();

    start();
    camera->setup(screenWidth, screenHeight);

    SDL_Init(SDL_INIT_EVERYTHING);

    while (screen->isOpen()) {

        // TODO: move event handling into another place
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            switch(e.type) {
                case SDL_QUIT:
                    screen->close();
                    exit();
                    return;
                case SDL_KEYDOWN:
                    Keyboard::sendKeyboardEvent(e);
                    break;
                case SDL_KEYUP:
                    Keyboard::sendKeyboardEvent(e);
                    break;
                case SDL_MOUSEMOTION:
                    Mouse::sendMouseEvent(e);
                    break;
            }
        }

        // 'd' in the beginning of the name means debug.
        // While printing debug info we will take into account only timer names witch start with 'd '
        Time::startTimer("d all");

        Time::startTimer("d clear");
        screen->clear();
        Time::stopTimer("d clear");

        Time::update();

        if(_updateWorld) {
            Time::startTimer("d animations");
            Timeline::update();
            Time::stopTimer("d animations");

            Time::startTimer("d collisions");
            world->update();
            Time::stopTimer("d collisions");
        }

        projectAndDrawGroup(world->objects());

        Time::stopTimer("d projections");
        Time::stopTimer("d rasterization");

        Time::startTimer("d game update");
        update();
        Time::stopTimer("d game update");

        Time::stopTimer("d all");

        printDebugInfo();
        screen->display();
    }
}

void Engine::exit() {
    if (screen->isOpen()) {
        screen->close();
    }

    Time::free();
    Timeline::free();
    Keyboard::free();
    Mouse::free();
    ResourceManager::free();

    Log::log("Engine::exit(): exit 3dzavr. Screen size: (" + std::to_string(screen->width()) + "x" + std::to_string(screen->height()) + ")");
}

void Engine::printDebugInfo() {
    if (_showDebugInfo) {
        // coordinates & fps:
        screen->drawText(Consts::BUILD_INFO, 15, 10);
        screen->drawText("fps: " + std::to_string(Time::fps()), 15, 30);
        screen->drawText("X: "   + std::to_string((camera->position().x())), 15, 45);
        screen->drawText("Y: "   + std::to_string((camera->position().y())), 15, 60);
        screen->drawText("Z: "   + std::to_string((camera->position().z())), 15, 75);
        screen->drawText("RY: "  + std::to_string(camera->angle().y()), 15, 90);
        screen->drawText("RL: "  + std::to_string(camera->angleLeftUpLookAt().x()), 15, 105);

        // timers:
        int timerWidth = 150;
        float xPos = 15;
        float yPos = 150;
        int height = 14;

        double totalTime = Time::elapsedTimerSeconds("d all");
        double timeSum = 0;
        int i = 0;

        if(!Time::timers()) {
            return;
        }
        for (auto &[timerName, timer] : Time::timers()->get()) {
            int width = timerWidth * timer.elapsedSeconds() / totalTime;

            if (timerName == "d all" || timerName[0] != 'd') {
                continue;
            }

            _histResources[timerName].emplace_back(Time::time(), timer.elapsedSeconds());

            screen->drawStrokeRectangle(xPos, yPos + (1.5*height)*i, width, height,
                                        Color(width * 255 / timerWidth, 255 - width * 255 / timerWidth, 0, 255));

            screen->drawText(
                    timerName.substr(2, timerName.size()) + " (" +
                    std::to_string((int) (100 * timer.elapsedSeconds() / totalTime)) + "%)",
                    xPos+5, yPos + (1.5*height)*i, 12, Consts::BLACK);

            screen->drawPlot(_histResources[timerName], timerWidth + 10, yPos + (1.5*height)*i, 50, height);

            i++;
            timeSum += timer.elapsedSeconds();
        }

        int width = timerWidth * (totalTime - timeSum) / totalTime;
        screen->drawStrokeRectangle(xPos, yPos + (1.5*height)*i, width, height,
                             Color(width * 255 / timerWidth, 255 - width * 255 / timerWidth, 0, 255));

        screen->drawText("all other stuff (" + std::to_string((int) (100 * (totalTime - timeSum) / totalTime)) + "%)",
                         xPos+5, yPos + (1.5*height)*i, 12, Color(0, 0, 0, 150));

        _histResources["d other"].emplace_back(Time::time(), totalTime - timeSum);
        screen->drawPlot(_histResources["d other"], timerWidth + 10, yPos + (1.5*height)*i, 50, height);

        // Draw a plot of fps

        _histResources["fpsCounter"].emplace_back(Time::time(), Time::fps());
        screen->drawPlot(_histResources["fpsCounter"], 100, 30, 50, 14);

        for(auto& [histName, data] : _histResources) {
            if(data.size() > 1000) {
                data.erase(data.begin());
            }
        }
    }
}
