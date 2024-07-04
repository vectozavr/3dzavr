#ifndef ENGINE_ENGINE_H
#define ENGINE_ENGINE_H

#include <io/Screen.h>
#include <utils/Log.h>
#include <objects/Camera.h>
#include <World.h>

/*
 * TODO: implement network system for the SDL version. That might be based on the SDL_net module:
 * https://wiki.libsdl.org/SDL2_net/FrontPage
 * https://wiki.libsdl.org/SDL2_net/CategoryAPI
 */

/**
 * @class Engine
 * @brief Main engine class responsible for managing the game's rendering, updating, and event handling.
 *
 * The Engine class encapsulates the core functionalities required for rendering objects, handling user input,
 * and managing game states. It uses SDL for event handling and supports dynamic lighting and debug information.
 */
class Engine {
private:
    bool _updateWorld = true; ///< Flag to control the update cycle of the world.

    std::vector<std::tuple<Triangle, Triangle, Material*>> _projectedOpaqueTriangles; ///< Stores projected opaque triangles for rendering.
    std::vector<std::tuple<Triangle, Triangle, Material*>> _projectedTranspTriangles; ///< Stores projected transparent triangles for rendering.
    std::vector<std::pair<Line, Color>> _projectedLines; ///< Stores lines to be rendered with specified colors.

    std::vector<std::shared_ptr<LightSource>> _lightSources; ///< Dynamic list of light sources in the scene.

    void projectObject(const Object& object); ///< Projects an object's geometry into screen space.
    void drawProjectedTriangles(); ///< Draws projected triangles onto the screen.

    // For debug purposes
    bool _showDebugInfo = Consts::SHOW_DEBUG_INFO; ///< Flag to control the display of debug information.
    std::map<std::string, std::vector<std::pair<double, double>>> _histResources; ///< Stores historical resource usage data for debugging.

    void printDebugInfo(); ///< Prints debug information to the console.

    static int handleSDLEvents(); ///< Handles SDL events, such as keyboard and mouse input.

protected:
    const std::shared_ptr<Screen> screen = std::make_shared<Screen>(); ///< Shared pointer to the screen object for rendering.

    const std::shared_ptr<World> world = std::make_shared<World>(ObjectTag("Main_Scene(World)")); ///< Shared pointer to the world object representing the game world.
    const std::shared_ptr<Camera> camera = std::make_shared<Camera>(); ///< Shared pointer to the camera object for viewing the world.

    virtual void start() {}; ///< Virtual method for initialization tasks.
    virtual void update() {}; ///< Virtual method for update tasks.

    [[nodiscard]] bool showDebugInfo() const { return _showDebugInfo; } ///< Getter for _showDebugInfo.
    void setDebugInfo(bool value) { _showDebugInfo = value; } ///< Setter for _showDebugInfo.

    void setUpdateWorld(bool value) { _updateWorld = value; } ///< Setter for _updateWorld.

    virtual void gui() {} ///< Virtual method for GUI tasks.

public:
    Engine(); ///< Constructor for the Engine class.

    void create(uint16_t screenWidth = Consts::STANDARD_SCREEN_WIDTH, uint16_t screenHeight = Consts::STANDARD_SCREEN_HEIGHT,
                const Color& background = Consts::BACKGROUND_COLOR); ///< Initializes the engine with specified screen dimensions and background color.

    void exit(); ///< Handles cleanup and exit procedures.
};


#endif //ENGINE_ENGINE_H
