#ifndef ENGINE_ENGINE_H
#define ENGINE_ENGINE_H

#include <io/Screen.h>
#include <utils/Log.h>
#include "objects/Camera.h"
#include <World.h>

/*
 * TODO: implement network system for the SDL version. That might be based on the SDL_net module:
 * https://wiki.libsdl.org/SDL2_net/FrontPage
 * https://wiki.libsdl.org/SDL2_net/CategoryAPI
 */

/*
 * TODO: implement different types of lighting: Point Lights, Spot Lights, Directional Lights, Area Lights
 * see examples on https://docs.unity3d.com/Manual/Lighting.html
 */



class Engine {
private:
    bool _updateWorld = true;
    [[maybe_unused]] void projectAndDrawGroup(const Group& group) const;

    std::vector<std::pair<Triangle, Material*>> _projectedOpaqueTriangles;
    std::vector<std::pair<Triangle, Material*>> _projectedTranspTriangles;
    void projectGroup(const Group& group);
    void drawProjectedTriangles();

    // For debug purposes
    bool _showDebugInfo = Consts::SHOW_DEBUG_INFO;
    std::map<std::string, std::vector<std::pair<double, double>>> _histResources;

    void printDebugInfo();

protected:
    const std::shared_ptr<Screen> screen = std::make_shared<Screen>();

    const std::shared_ptr<World> world = std::make_shared<World>();
    const std::shared_ptr<Camera> camera = std::make_shared<Camera>();

    virtual void start() {};
    virtual void update() {};

    [[nodiscard]] bool showDebugInfo() const { return _showDebugInfo; }
    void setDebugInfo(bool value) { _showDebugInfo = value; }

    void setUpdateWorld(bool value) { _updateWorld = value; }

    virtual void gui() {}

public:
    Engine();

    void create(uint16_t screenWidth = Consts::STANDARD_SCREEN_WIDTH, uint16_t screenHeight = Consts::STANDARD_SCREEN_HEIGHT,
                const Color& background = Consts::BACKGROUND_COLOR);

    void exit();
};


#endif //ENGINE_ENGINE_H
