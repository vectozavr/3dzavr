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

void Engine::create(uint16_t screenWidth, uint16_t screenHeight, const std::string &name, const Color& background) {

    _name = name;
    screen->open(screenWidth, screenHeight, name, background);

    Log::log("Engine::create(): started engine (" + std::to_string(screenWidth) + "x" + std::to_string(screenHeight) +
             ") with title '" + name + "'.");
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

        screen->clear();

        Time::update();



        if(_updateWorld) {
            Time::startTimer("d animations");
            Timeline::update();
            Time::stopTimer("d animations");

            Time::startTimer("d collisions");
            world->update();
            Time::stopTimer("d collisions");
        }

        for (const auto &[groupTag, group] : *world) {
            for(const auto& [objTag, obj] : *group) {
                // TODO: add support for groups of groups of objects (recursive call of the projection function)

                std::shared_ptr<Mesh> mesh = std::dynamic_pointer_cast<Mesh>(obj);
                if(mesh) {
                    // project triangles to the camera plane
                    Time::startTimer("d projections");
                    auto projected = camera->project(mesh);
                    Time::pauseTimer("d projections");

                    // draw projected triangles
                    Time::startTimer("d rasterization");
                    for (auto &t : projected) {
                        screen->drawTriangle(*t.first, t.second);
                    }
                    Time::pauseTimer("d rasterization");
                }
            }
        }

        Time::stopTimer("d projections");
        Time::stopTimer("d rasterization");

        Time::startTimer("d depthBuffer");
        // draw triangles on the screen
        screen->clearDepthBuffer();
        Time::stopTimer("d depthBuffer");
        Time::stopTimer("d all");


        Time::startTimer("d game update");
        update();
        Time::stopTimer("d game update");

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

    Log::log("Engine::exit(): exit engine (" + std::to_string(screen->width()) + "x" +
             std::to_string(screen->height()) + ") with title '" + screen->title() + "'.");
}

void Engine::printDebugInfo() const {
    if (_showDebugInfo) {
        // coordinates & fps:
        std::string text = _name + "\n\n X: " +
                           std::to_string((camera->position().x())) + "\n Y: " +
                           std::to_string((camera->position().y())) + "\n Z: " +
                           std::to_string((camera->position().z())) + "\n RY:" +
                            std::to_string(camera->angle().y()) + "\n RL: " +
                            std::to_string(camera->angleLeftUpLookAt().x()) + "\n\n" +
                           std::to_string(screen->width()) + "x" +
                           std::to_string(screen->height()) + " " +
                           std::to_string(Time::fps()) + " fps";


        screen->drawText("fps: " + std::to_string(Time::fps()), 10, 10);
        screen->drawText("X: " + std::to_string((camera->position().x())), 10, 30);
        screen->drawText("Y: " + std::to_string((camera->position().y())), 10, 50);
        screen->drawText("Z: " + std::to_string((camera->position().z())), 10, 70);
        screen->drawText("RY: " +std::to_string(camera->angle().y()), 10, 90);
        screen->drawText("RL: " +std::to_string(camera->angleLeftUpLookAt().x()), 10, 110);

        // timers:
        int timerWidth = screen->width()*2/3;
        float xPos = screen->width()/4;
        float yPos = screen->height()/2;
        int height = screen->height()/30;

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

            screen->drawStrokeRectangle(xPos, yPos + (1.5*height)*i, width, height,
                                        Color( std::array<double, 4>{(float)(width) / timerWidth, (1.0f - (float)(width) / timerWidth), 0, 1}));

            screen->drawText(
                    timerName.substr(2, timerName.size()) + " (" +
                    std::to_string((int) (100 * timer.elapsedSeconds() / totalTime)) + "%)",
                    10, yPos + (1.5*height)*i, 12, Color(0, 0, 0, 150));

            i++;
            timeSum += timer.elapsedSeconds();
        }

        int width = timerWidth * (totalTime - timeSum) / totalTime;
        screen->drawStrokeRectangle(xPos, yPos + (1.5*height)*i, width, height,
                             Color( std::array<double, 4>{(float)(width) / timerWidth, (1.0f - (float)(width) / timerWidth), 0, 1}));

        screen->drawText("all other stuff (" + std::to_string((int) (100 * (totalTime - timeSum) / totalTime)) + "%)",
                         10, yPos + (1.5*height)*i, 12, Color(0, 0, 0, 150));

    }
}
