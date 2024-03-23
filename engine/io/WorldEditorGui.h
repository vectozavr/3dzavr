#ifndef IO_GUI_H
#define IO_GUI_H

#include <memory>
#include <functional>
#include "SDL.h"

extern "C" {
#include "io/microui/microui.h"
}

class WorldEditorGui {
private:
    std::unique_ptr<mu_Context> _ctx;

    // Function for drawing
    std::function<void(bool)> _setDepthTest;
    std::function<void(int, int, int, int, const Color&, uint16_t)> _drawLine;
    std::function<void(int, int, uint16_t, uint16_t, const Color&)> _drawRectangle;
    std::function<void(const std::string&, int, int, const Color&)> _drawText;

    void handleInputEvents();
    void drawUiIcon(int id, uint16_t x, uint16_t y);
    void processFrame();
    void renderGui();

    void renderSettings();

    bool _enableLighting = true;
    bool _enableTrueLighting = false;
    bool _enableTransparency = true;
    bool _enableTriangleBorders = false;
    bool _enableTexturing = true;
    bool _enableMipmapping = true;
    bool _enableDepthTest = true;
public:
    WorldEditorGui(const std::function<void(bool)>& setDepthTestFunction,
                   const std::function<void(int, int, int, int, const Color&, uint16_t)>& drawLineFunction,
                   const std::function<void(int, int, uint16_t, uint16_t, const Color&)>& drawRectangleFunction,
                   const std::function<void(const std::string&, int, int, const Color&)>& drawTextFunction);

    void update();

    [[nodiscard]] inline bool isEnabledLighting() const { return _enableLighting; }
    [[nodiscard]] inline bool isEnabledTrueLighting() const { return _enableTrueLighting; }
    [[nodiscard]] inline bool isEnabledTransparency() const { return _enableTransparency; }
    [[nodiscard]] inline bool isEnabledTriangleBorders() const { return _enableTriangleBorders; }
    [[nodiscard]] inline bool isEnabledTexturing() const { return _enableTexturing; }
    [[nodiscard]] inline bool isEnabledMipmapping() const { return _enableMipmapping; }
    [[nodiscard]] inline bool isEnabledDepthTest() const { return _enableDepthTest; }
};


#endif //IO_GUI_H