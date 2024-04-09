#ifndef UTILS_WORLDEDITOR_H
#define UTILS_WORLDEDITOR_H

#include <utils/ObjectController.h>
#include <io/Screen.h>
#include <World.h>


extern "C" {
#include "io/microui/microui.h"
}


class WorldEditor {
private:
    std::unique_ptr<mu_Context> _ctx;

    std::shared_ptr<Screen> _screen;
    std::shared_ptr<World> _world;
    std::shared_ptr<Camera> _camera;

    std::shared_ptr<Object> _selectedObject = nullptr;
    std::shared_ptr<Object> _selectedObjectBounds = nullptr;
    std::shared_ptr<Object> _redCube = nullptr;

    std::shared_ptr<ObjectController> _objController = nullptr;
    std::shared_ptr<ObjectController> _cameraController = nullptr;

    bool _enableLighting = true;
    bool _enableTrueLighting = false;
    bool _enableTransparency = true;
    bool _enableTriangleBorders = false;
    bool _enableTexturing = true;
    bool _enableMipmapping = true;
    bool _enableDepthTest = true;

    bool _objInFocus = false;
    bool _isControllerActive = true;
    bool _isRecording = false;

    int _operation = -1;

    float _lightingLODNearDistance = Consts::LIGHTING_LOD_NEAR_DISTANCE;
    float _lightingLODFarDistance = Consts::LIGHTING_LOD_FAR_DISTANCE;

    void handleInputEvents();
    void drawUiIcon(int id, uint16_t x, uint16_t y);
    void processFrame();
    void renderGui();

    void controlPanel();
    void objectTree(const std::shared_ptr<Object>& object);
    void renderSettings();

    void objectEditor();
    void transformMatrixEditor();
    void triangleMeshEditor();
    void lineMeshEditor();
    void lightSourceEditor();
    void rigidObjectEditor();


    void updateScreenSettings();
    void updateControllers();
public:
    WorldEditor(std::shared_ptr<Screen> screen, std::shared_ptr<World> world, std::shared_ptr<Camera> camera);

    void update();

    [[nodiscard]] std::shared_ptr<Object> selectedObject() { return _selectedObject; }
};


#endif //UTILS_WORLDEDITOR_H
