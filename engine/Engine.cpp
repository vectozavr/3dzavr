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
        std::shared_ptr<TriangleMesh> subMesh = std::dynamic_pointer_cast<TriangleMesh>(obj);
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
        std::shared_ptr<LineMesh> subLineMesh = std::dynamic_pointer_cast<LineMesh>(obj);
        if(subLineMesh) {
            auto projectedLines = camera->project(*subLineMesh);
            for(const auto& projectedLine: projectedLines) {
                _projectedLines.emplace_back(projectedLine, subLineMesh->getColor());
            }
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
        const auto& [projT2, t2, material2] = e2;

        double z1 = projT1[0].z() + projT1[1].z() + projT1[2].z();
        double z2 = projT2[0].z() + projT2[1].z() + projT2[2].z();
        return z1 > z2;
    });
    Time::stopTimer("d sort triangles");

    Time::startTimer("d rasterization");
    auto cameraPosition = camera->fullPosition();
    // Draw opaque (non-transparent) triangles
    for (const auto& [projectedTriangle, triangle, material]: _projectedOpaqueTriangles) {
        screen->drawTriangleWithLighting(projectedTriangle, triangle, _lightSources, cameraPosition, material);
    }
    // Draw transparent triangles
    for (const auto& [projectedTriangle, triangle, material]: _projectedTranspTriangles) {
        screen->drawTriangleWithLighting(projectedTriangle, triangle, _lightSources, cameraPosition, material);
    }
    // Draw lines
    for (const auto& [line, color]: _projectedLines) {
        screen->drawLine(line, color);
    }

    Time::stopTimer("d rasterization");
}

int Engine::handleSDLEvents() {
    SDL_Event e;

    while (SDL_PollEvent(&e)) {
        switch(e.type) {
            case SDL_QUIT:
                return 1;
            case SDL_KEYDOWN:
            case SDL_KEYUP:
            case SDL_TEXTINPUT:
                Keyboard::sendKeyboardEvent(e);
                break;
            case SDL_MOUSEMOTION:
            case SDL_MOUSEWHEEL:
            case SDL_MOUSEBUTTONDOWN:
            case SDL_MOUSEBUTTONUP:
                Mouse::sendMouseEvent(e);
                break;
        }
    }

    return 0;
}

void Engine::create(uint16_t screenWidth, uint16_t screenHeight, const Color& background) {

    screen->open(Consts::STANDARD_EDITOR_WIDTH, Consts::STANDARD_EDITOR_HEIGHT, background);

    Log::log("Engine::create(): started 3dzavr. Screen size: (" + std::to_string(screenWidth) + "x" + std::to_string(screenHeight) + ")");
    Time::update();

    screen->setDepthTest(true);

    start();
    camera->setup(screenWidth, screenHeight);

    SDL_Init(SDL_INIT_EVERYTHING);

    while (screen->isOpen()) {

        Time::update();

        Keyboard::clear();
        Mouse::clear();

        if(handleSDLEvents()) {
            screen->close();
            exit();
            return;
        };

        // 'd' in the beginning of the name means debug.
        // While printing debug info we will take into account only timer names witch start with 'd '
        Time::startTimer("d all");

        Time::startTimer("d clear");
        screen->clear();
        Time::stopTimer("d clear");

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
        _projectedLines.clear();

        Time::stopTimer("d projections");
        Time::stopTimer("d rasterization");

        Time::stopTimer("d all");

        printDebugInfo();

        update();

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
        float xPos = 10;
        float yPos = 220;
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
                    xPos+5, yPos + (1.5*height)*i, Color::BLACK, 12);

            screen->drawPlot(_histResources[timerName], xPos + timerWidth, yPos + (1.5*height)*i, plotWidth, height);

            i++;
            timeSum += timer.elapsedSeconds();
        }

        int width = timerWidth * (totalTime - timeSum) / totalTime;
        screen->drawStrokeRectangle(xPos, yPos + (1.5*height)*i, width, height,
                             Color(width * 255 / timerWidth, 255 - width * 255 / timerWidth, 0, 255));

        screen->drawText("all other stuff (" + std::to_string((int) (100 * (totalTime - timeSum) / totalTime)) + "%)",
                         xPos+5, yPos + (1.5*height)*i, Color(0, 0, 0, 150), 12);

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
