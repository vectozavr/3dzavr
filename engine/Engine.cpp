#include <iostream>

#include <Engine.h>
#include <utils/Time.h>
#include <utils/ResourceManager.h>
#include <animation/Timeline.h>
#include <io/Keyboard.h>
#include <io/Mouse.h>
#include <utils/monitoring.h>

Engine::Engine() {
    Time::init();
    Timeline::init();
    Keyboard::init();
    Mouse::init();
    ResourceManager::init();
}

void Engine::projectGroup(const Group &group) {
    for(const auto& [objTag, obj] : group) {
        std::shared_ptr<Mesh> subMesh = std::dynamic_pointer_cast<Mesh>(obj);
        if(subMesh) {
            auto projected = camera->project(*subMesh);
            std::shared_ptr<Material> material = subMesh->getMaterial();
            bool isTransparent = material->isTransparent();

            if(!isTransparent) {
                for(const auto& [projectedTriangle, triangle]: projected) {
                    _projectedOpaqueTriangles.emplace_back(projectedTriangle, triangle, material.get());
                }
            } else {
                for(const auto& [projectedTriangle, triangle]: projected) {
                    _projectedTranspTriangles.emplace_back(projectedTriangle, triangle, material.get());
                }
            }
        }
        std::shared_ptr<Group> subGroup = std::dynamic_pointer_cast<Group>(obj);
        if(subGroup) {
            // We need to recursively continue to draw subgroup
            projectGroup(*subGroup);
        }

        std::shared_ptr<LightSource> l = std::dynamic_pointer_cast<LightSource>(obj);
        if(l) {
            _lightSources.emplace_back(l);
        }
    }
}

void Engine::drawProjectedTriangles() {

    Time::startTimer("d sort triangles");
    std::sort(_projectedTranspTriangles.begin(), _projectedTranspTriangles.end(), [](const auto& e1, const auto& e2){
        const auto& [projT1, t1, material1] = e1;
        const auto& [projT2, t2, material2] = e1;

        double z1 = projT1[0].z() + projT1[1].z() + projT1[2].z();
        double z2 = projT2[0].z() + projT2[1].z() + projT2[2].z();
        return z1 > z2;
    });
    Time::stopTimer("d sort triangles");

    Time::startTimer("d rasterization");
    for (const auto& [projectedTriangle, triangle, material]: _projectedOpaqueTriangles) {
        screen->drawTriangleWithLighting(projectedTriangle, triangle, _lightSources, material);
        //screen->drawTriangle(projectedTriangle, material);
    }
    for (const auto& [projectedTriangle, triangle, material]: _projectedTranspTriangles) {
        screen->drawTriangleWithLighting(projectedTriangle, triangle, _lightSources, material);
        //screen->drawTriangle(projectedTriangle, material);
    }
    Time::stopTimer("d rasterization");
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

        _lightSources.clear();

        Time::startTimer("d projections");
        projectGroup(*world->objects());
        Time::stopTimer("d projections");

        drawProjectedTriangles();
        _projectedOpaqueTriangles.clear();
        _projectedTranspTriangles.clear();

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
        uint8_t offset = 10;
        uint8_t shift = 0;
        uint8_t h = 15;

        // Build info, coordinates & fps:
        screen->drawText(Consts::BUILD_INFO, 10, (shift++)*h + offset);
        screen->drawText("System: " + Consts::OPERATION_SYSTEM + ", " + Consts::CPU_ARCHITECTURE, 10, (shift++)*h + offset);
        shift++;
        screen->drawText("fps: " + std::to_string(Time::fps()), 10, (shift++)*h + offset);
        shift++;
        screen->drawText("X: "   + std::to_string((camera->position().x())), 10, (shift++)*h + offset);
        screen->drawText("Y: "   + std::to_string((camera->position().y())), 10, (shift++)*h + offset);
        screen->drawText("Z: "   + std::to_string((camera->position().z())), 10, (shift++)*h + offset);
        screen->drawText("RY: "  + std::to_string(camera->angle().y()), 10, (shift++)*h + offset);
        screen->drawText("RL: "  + std::to_string(camera->angleLeftUpLookAt().x()), 10, (shift++)*h + offset);
        shift++;

        //Process info:
        auto res = getProcessSizeMB();
        screen->drawText("Process size: " + std::to_string(res) + " MB", 10, (shift++)*h + offset);

        // timers:
        int timerWidth = 150;
        int plotWidth = 50;
        float xPos = screen->width() - timerWidth - plotWidth - 10;
        float yPos = 10;
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
                    xPos+5, yPos + (1.5*height)*i, 12, Color::BLACK);

            screen->drawPlot(_histResources[timerName], xPos + timerWidth, yPos + (1.5*height)*i, plotWidth, height);

            i++;
            timeSum += timer.elapsedSeconds();
        }

        int width = timerWidth * (totalTime - timeSum) / totalTime;
        screen->drawStrokeRectangle(xPos, yPos + (1.5*height)*i, width, height,
                             Color(width * 255 / timerWidth, 255 - width * 255 / timerWidth, 0, 255));

        screen->drawText("all other stuff (" + std::to_string((int) (100 * (totalTime - timeSum) / totalTime)) + "%)",
                         xPos+5, yPos + (1.5*height)*i, 12, Color(0, 0, 0, 150));

        _histResources["d other"].emplace_back(Time::time(), totalTime - timeSum);
        screen->drawPlot(_histResources["d other"], xPos + timerWidth, yPos + (1.5*height)*i, 50, height);

        // Draw a plot of fps

        _histResources["fpsCounter"].emplace_back(Time::time(), Time::fps());
        screen->drawPlot(_histResources["fpsCounter"], 60, 3*h + offset, plotWidth, 14);

        for(auto& [histName, data] : _histResources) {
            if(data.size() > 1000) {
                data.erase(data.begin());
            }
        }
    }
}
