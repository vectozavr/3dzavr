#ifndef IO_GUI_H
#define IO_GUI_H

#include <memory>
#include <functional>
#include "SDL.h"

#include <objects/Group.h>

extern "C" {
#include "io/microui/microui.h"
}

class WorldEditorGui {
private:
    std::unique_ptr<mu_Context> _ctx;
    std::shared_ptr<Group> _objects;
    std::shared_ptr<Object> _selectedObject = nullptr;

    // Function for drawing
    std::function<void(bool)> _setDepthTest;
    std::function<void(int, int, int, int, const Color&, uint16_t)> _drawLine;
    std::function<void(int, int, uint16_t, uint16_t, const Color&)> _drawRectangle;
    std::function<void(const std::string&, int, int, const Color&)> _drawText;

    void handleInputEvents();
    void drawUiIcon(int id, uint16_t x, uint16_t y);
    void processFrame();
    void renderGui();

    void controlPanel();
    void groupTree(const std::shared_ptr<Group>& group);
    void objectEditor();
    void renderSettings();

    int _operation = -1;

    bool _enableLighting = true;
    bool _enableTrueLighting = false;
    bool _enableTransparency = true;
    bool _enableTriangleBorders = false;
    bool _enableTexturing = true;
    bool _enableMipmapping = true;
    bool _enableDepthTest = true;

    float _lightingLODNearDistance = Consts::LIGHTING_LOD_NEAR_DISTANCE;
    float _lightingLODFarDistance = Consts::LIGHTING_LOD_FAR_DISTANCE;
public:
    WorldEditorGui(const std::function<void(bool)>& setDepthTestFunction,
                   const std::function<void(int, int, int, int, const Color&, uint16_t)>& drawLineFunction,
                   const std::function<void(int, int, uint16_t, uint16_t, const Color&)>& drawRectangleFunction,
                   const std::function<void(const std::string&, int, int, const Color&)>& drawTextFunction,
                   const std::shared_ptr<Group>& objects);

    void update();

    [[nodiscard]] inline bool isEnabledLighting() const { return _enableLighting; }
    [[nodiscard]] inline bool isEnabledTrueLighting() const { return _enableTrueLighting; }
    [[nodiscard]] inline bool isEnabledTransparency() const { return _enableTransparency; }
    [[nodiscard]] inline bool isEnabledTriangleBorders() const { return _enableTriangleBorders; }
    [[nodiscard]] inline bool isEnabledTexturing() const { return _enableTexturing; }
    [[nodiscard]] inline bool isEnabledMipmapping() const { return _enableMipmapping; }
    [[nodiscard]] inline bool isEnabledDepthTest() const { return _enableDepthTest; }

    [[nodiscard]] std::shared_ptr<Object> selectedObject() { return _selectedObject; }

    void setSelectedObject(const std::shared_ptr<Object>& object) { _selectedObject = object; }

    [[nodiscard]] inline double getLightingLODNearDistance() const { return _lightingLODNearDistance; }
    [[nodiscard]] inline double getLightingLODFarDistance() const { return _lightingLODFarDistance; }
};


#endif //IO_GUI_H
