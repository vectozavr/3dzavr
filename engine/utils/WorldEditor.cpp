#include <utils/ResourceManager.h>
#include <utils/WorldEditor.h>
#include <utils/Log.h>
#include <io/Keyboard.h>
#include <io/Mouse.h>
#include <components/lighting/SpotLight.h>

#include <utility>

static const char button_map[256] = {
        [ SDL_BUTTON_LEFT   & 0xff ] =  MU_MOUSE_LEFT,
        [ SDL_BUTTON_RIGHT  & 0xff ] =  MU_MOUSE_RIGHT,
        [ SDL_BUTTON_MIDDLE & 0xff ] =  MU_MOUSE_MIDDLE,
};

static const char key_map[256] = {
        [ SDLK_LSHIFT       & 0xff ] = MU_KEY_SHIFT,
        [ SDLK_RSHIFT       & 0xff ] = MU_KEY_SHIFT,
        [ SDLK_LCTRL        & 0xff ] = MU_KEY_CTRL,
        [ SDLK_RCTRL        & 0xff ] = MU_KEY_CTRL,
        [ SDLK_LALT         & 0xff ] = MU_KEY_ALT,
        [ SDLK_RALT         & 0xff ] = MU_KEY_ALT,
        [ SDLK_RETURN       & 0xff ] = MU_KEY_RETURN,
        [ SDLK_BACKSPACE    & 0xff ] = MU_KEY_BACKSPACE,
};

int textWidth(mu_Font font, const char *text, int len) {
    if (len == -1) { len = strlen(text); }
    auto f = ResourceManager::loadFont(Consts::DEFAULT_FONT_FILENAME);
    int res = 0;
    for (const char *p = text; *p && len--; p++) {
        if ((*p & 0xc0) == 0x80) { continue; }
        res += f->getTextSize(std::string(1, *p), 12).first;
    }
    return res;
}

int textHeight(mu_Font font) {
    return 15;
}

WorldEditor::WorldEditor(std::shared_ptr<Screen> screen, std::shared_ptr<World> world, std::shared_ptr<Camera> camera):
        _screen(std::move(screen)), _world(std::move(world)), _camera(std::move(camera)) {
    _cameraController = std::make_shared<ObjectController>(_camera->transformMatrix());

    _selectedObjectBounds = std::make_shared<Object>(ObjectTag("Cube_frame"));
    auto lineMeshComponent = _selectedObjectBounds->addComponent<LineMesh>(LineMesh::Cube());
    lineMeshComponent->setVisible(false);

    _world->add(_selectedObjectBounds);

    _ctx = std::make_unique<mu_Context>();
    mu_init(_ctx.get());
    _ctx->text_width = textWidth;
    _ctx->text_height = textHeight;

    //_ctx.get()->style->colors[MU_COLOR_WINDOWBG] = {50, 50, 50, 200};

    // For debug
    _redCube = std::make_shared<Object>(ObjectTag("RedCube"));
    auto triangleMeshComponent = _redCube->addComponent<TriangleMesh>(TriangleMesh::Cube(0.1));
    triangleMeshComponent->setVisible(_objInFocus);

#ifndef NDEBUG
    _world->add(_redCube);
#endif
}

void WorldEditor::handleInputEvents() {
    auto pos = Mouse::getMousePosition() / Consts::SCREEN_SCALE;
    mu_input_mousemove(_ctx.get(), (int) pos.x(), (int) pos.y());
    mu_input_scroll(_ctx.get(), 0, (int) Mouse::getMouseScroll().y() * -30);
    // Mouse buttons events:
    if (Mouse::isButtonDown()) {
        auto buttons = Mouse::buttonsDown();
        for (const auto &b: buttons) {
            mu_input_mousedown(_ctx.get(), (int) pos.x(), (int) pos.y(), button_map[b & 0xff]);
        }
    }
    if (Mouse::isButtonUp()) {
        auto buttons = Mouse::buttonsUp();
        for (const auto &b: buttons) {
            mu_input_mouseup(_ctx.get(), (int) pos.x(), (int) pos.y(), button_map[b & 0xff]);
        }
    }
    // Keyboard keys events
    if (Keyboard::isKeyDown()) {
        auto keys = Keyboard::keysDown();
        for (const auto &k: keys) {
            mu_input_keydown(_ctx.get(), key_map[k & 0xff]);
        }
    }
    if (Keyboard::isKeyUp()) {
        auto keys = Keyboard::keysUp();
        for (const auto &k: keys) {
            mu_input_keyup(_ctx.get(), key_map[k & 0xff]);
        }
    }
    mu_input_text(_ctx.get(), Keyboard::inputText().c_str());
}

void WorldEditor::drawUiIcon(int id, uint16_t x, uint16_t y) {
    switch (id) {
        case MU_ICON_CLOSE:
            _screen->drawLine(x+9, y+7, x+16, y+14, Color::WHITE_GRAY, 2);
            _screen->drawLine(x+9, y+14, x+16, y+7, Color::WHITE_GRAY, 2);
            break;
        case MU_ICON_CHECK:
            _screen->drawLine(x+7, y+14, x+4, y+11, Color::WHITE_GRAY, 2);
            _screen->drawLine(x+7, y+14, x+15, y+6, Color::WHITE_GRAY, 2);
            break;
        case MU_ICON_COLLAPSED:
            _screen->drawLine(x+7, y+14, x+12, y+10, Color::WHITE_GRAY, 2);
            _screen->drawLine(x+7, y +6, x+12, y+10, Color::WHITE_GRAY, 2);
            break;
        case MU_ICON_EXPANDED:
            _screen->drawLine(x+6, y+8, x+10, y+12, Color::WHITE_GRAY, 2);
            _screen->drawLine(x+14, y+8, x+10, y+12, Color::WHITE_GRAY, 2);
            break;
        default:
            break;
    }
}

void WorldEditor::processFrame() {
    mu_begin(_ctx.get());

    controlPanel();

    mu_end(_ctx.get());
}

void WorldEditor::renderGui() {
    _screen->setDepthTest(false);
    mu_Command *cmd = nullptr;
    while (mu_next_command(_ctx.get(), &cmd)) {
        switch (cmd->type) {
            case MU_COMMAND_TEXT:
                _screen->drawText(cmd->text.str, cmd->text.pos.x, cmd->text.pos.y,
                          Color(cmd->text.color.r, cmd->text.color.g, cmd->text.color.b, cmd->text.color.a));

                break;
            case MU_COMMAND_RECT:
                _screen->drawRectangle(cmd->rect.rect.x, cmd->rect.rect.y, cmd->rect.rect.w, cmd->rect.rect.h,
                               Color(cmd->rect.color.r, cmd->rect.color.g, cmd->rect.color.b, cmd->rect.color.a));
                break;
            case MU_COMMAND_ICON:
                drawUiIcon(cmd->icon.id, cmd->icon.rect.x, cmd->icon.rect.y);
                break;
        }
    }
    _screen->setDepthTest(true);
}

void WorldEditor::controlPanel() {
    auto ctx = _ctx.get();

    if (mu_begin_window(ctx, "Control Panel", mu_rect(Consts::STANDARD_SCREEN_WIDTH, 0,
                                                      Consts::STANDARD_EDITOR_WIDTH-Consts::STANDARD_SCREEN_WIDTH, Consts::STANDARD_SCREEN_HEIGHT))) {

        objectEditor();


        if (mu_begin_treenode(ctx, "World tree")) {
            objectTree(_world);

            mu_end_treenode(ctx);
        }

        renderSettings();

        if (mu_begin_treenode_ex(ctx, "Info", MU_OPT_EXPANDED)) {
            mu_text(ctx, Consts::BUILD_INFO.c_str());
            mu_text(ctx, ("System: " + Consts::OPERATION_SYSTEM + ", " + Consts::CPU_ARCHITECTURE).c_str());
            mu_text(ctx, "Press 'q' to enter/quit free camera mode");

            mu_end_treenode(ctx);
        }

        mu_end_window(ctx);
    }
}

void WorldEditor::objectTree(const std::shared_ptr<Object> &object) {
    auto ctx = _ctx.get();

    if (mu_begin_treenode(ctx, object->name().str().c_str())) {

        mu_layout_row(ctx, 1, (int[]) { 200 }, 0);
        if (object != _world && mu_button_ex(ctx, "Select the whole group", MU_ICON_CHECK, 1)) {
            _selectedObject = object;
        }

        mu_text(ctx, "The content of the group");
        for(const auto& [objTag, obj] : *object) {
            if(obj->numberOfAttached() > 0) {
                // We need to recursively continue to draw subgroup
                objectTree(obj);
            } else {
                mu_layout_row(ctx, 1, (int[]) { 200 }, 0);
                if (mu_button(ctx, objTag.str().c_str())) {
                    _selectedObject = obj;
                }
            }
        }

        mu_end_treenode(ctx);
    }
}

void WorldEditor::transformMatrixEditor() {
    auto ctx = _ctx.get();

    auto transformMatrix = _selectedObject->getComponent<TransformMatrix>();
    if(!transformMatrix) {
        return;
    }

    if (mu_begin_treenode(ctx, "Transform Matrix")) {

        auto pos = transformMatrix->fullPosition();
        mu_text(ctx, ("X / Y / Z: " + std::to_string(pos.x()) + " / " + std::to_string(pos.y()) + " / " +
                      std::to_string(pos.z())).c_str());

        mu_layout_row(ctx, 3, (int[]) {60, 60, 60}, 0);
        if (mu_button(ctx, "Translate")) { _operationTransformMatrix = 0; }
        if (mu_button(ctx, "Scale")) { _operationTransformMatrix = 1; }
        if (mu_button(ctx, "Rotate")) { _operationTransformMatrix = 2; }

        float x = 0;
        float y = 0;
        float z = 0;

        switch (_operationTransformMatrix) {
            case 0:
                mu_layout_begin_column(ctx);
                mu_layout_row(ctx, 2, (int[]) {70, 150}, 0);
                mu_label(ctx, "Translate X:");
                mu_slider(ctx, &x, -30, 30);
                mu_label(ctx, "Translate Y:");
                mu_slider(ctx, &y, -30, 30);
                mu_label(ctx, "Translate Z:");
                mu_slider(ctx, &z, -30, 30);
                mu_layout_end_column(ctx);
                transformMatrix->translate(Vec3D(x, y, z) * Time::deltaTime());
                break;
            case 1:
                mu_layout_begin_column(ctx);
                mu_layout_row(ctx, 2, (int[]) {70, 150}, 0);
                mu_label(ctx, "Scale X:");
                mu_slider(ctx, &x, -3, 3);
                mu_label(ctx, "Scale Y:");
                mu_slider(ctx, &y, -3, 3);
                mu_label(ctx, "Scale Z:");
                mu_slider(ctx, &z, -3, 3);
                mu_layout_end_column(ctx);
                transformMatrix->scaleInside(Vec3D(1 + x * Time::deltaTime(),
                                                   1 + y * Time::deltaTime(),
                                                   1 + z * Time::deltaTime()));
                break;
            case 2:
                mu_layout_begin_column(ctx);
                mu_layout_row(ctx, 2, (int[]) {70, 150}, 0);
                mu_label(ctx, "Rotate X:");
                mu_slider(ctx, &x, -3, 3);
                mu_label(ctx, "Rotate Y:");
                mu_slider(ctx, &y, -3, 3);
                mu_label(ctx, "Rotate Z:");
                mu_slider(ctx, &z, -3, 3);
                mu_layout_end_column(ctx);
                transformMatrix->rotateRelativePoint(_selectedObject->getComponent<TransformMatrix>()->position(),
                                                     Vec3D(x, y, z) * Time::deltaTime());
                break;
        }

        mu_layout_row(ctx, 1, (int[]) {120}, 0);
        if (mu_button(ctx, "Undo Transform")) {
            transformMatrix->transform(_selectedObject->getComponent<TransformMatrix>()->invModel());
        }

        mu_end_treenode(ctx);
    }
}

void WorldEditor::triangleMeshEditor() {
    auto ctx = _ctx.get();

    auto triangleMesh = _selectedObject->getComponent<TriangleMesh>();
    if(!triangleMesh) {
        return;
    }

    if (mu_begin_treenode(ctx, "Triangle Mesh")) {

        mu_label(ctx, ("Number of triangles: " + std::to_string(triangleMesh->size()/3)).c_str());
        mu_label(ctx, ("Number of points: " + std::to_string(triangleMesh->size())).c_str());

        auto material = triangleMesh->getMaterial();
        mu_label(ctx, ("Material: " + material->tag().str()).c_str());

        auto texture = material->texture();
        if(texture) {
            mu_label(ctx, ("Texture: " + texture->fileName().str()).c_str());
        } else {
            static float color[3] = { static_cast<float>(material->ambient().r()),
                                      static_cast<float>(material->ambient().g()),
                                      static_cast<float>(material->ambient().b())};
            mu_label(ctx, "Ambient color");

            mu_layout_begin_column(ctx);
            mu_layout_row(ctx, 2, (int[]) { 46, -1 }, 0);
            mu_label(ctx, "Red:");   mu_slider(ctx, &color[0], 0, 255);
            mu_label(ctx, "Green:"); mu_slider(ctx, &color[1], 0, 255);
            mu_label(ctx, "Blue:");  mu_slider(ctx, &color[2], 0, 255);
            mu_layout_end_column(ctx);
            material->setAmbient(Color(color[0], color[1], color[2]));
        }

        float d = material->d();
        mu_layout_begin_column(ctx);
        mu_layout_row(ctx, 2, (int[]) {100, 130}, 0);
        mu_label(ctx, "Transparency:"); mu_slider(ctx, &d, 0, 1);
        mu_layout_end_column(ctx);
        material->setTransparency(d);

        bool visible = triangleMesh->isVisible();
        mu_checkbox(ctx, "Visible", &visible);
        triangleMesh->setVisible(visible);

        mu_end_treenode(ctx);
    }
}

void WorldEditor::lineMeshEditor() {
    auto ctx = _ctx.get();

    auto lineMesh = _selectedObject->getComponent<LineMesh>();
    if(!lineMesh) {
        return;
    }

    if (mu_begin_treenode(ctx, "Line Mesh")) {
        mu_label(ctx, ("Number of lines: " + std::to_string(lineMesh->size()/2)).c_str());
        mu_label(ctx, ("Number of points: " + std::to_string(lineMesh->size())).c_str());

        static float color[3] = { static_cast<float>(lineMesh->getColor().r()),
                                  static_cast<float>(lineMesh->getColor().g()),
                                  static_cast<float>(lineMesh->getColor().b())};
        mu_label(ctx, "Color");

        mu_layout_begin_column(ctx);
        mu_layout_row(ctx, 2, (int[]) { 46, -1 }, 0);
        mu_label(ctx, "Red:");   mu_slider(ctx, &color[0], 0, 255);
        mu_label(ctx, "Green:"); mu_slider(ctx, &color[1], 0, 255);
        mu_label(ctx, "Blue:");  mu_slider(ctx, &color[2], 0, 255);
        mu_layout_end_column(ctx);

        lineMesh->setColor(Color(color[0], color[1], color[2]));

        bool visible = lineMesh->isVisible();
        mu_checkbox(ctx, "Visible", &visible);
        lineMesh->setVisible(visible);

        mu_end_treenode(ctx);
    }
}

void WorldEditor::lightSourceEditor() {
    auto ctx = _ctx.get();

    auto lightSource = _selectedObject->getComponent<LightSource>();
    if(!lightSource) {
        return;
    }

    if (mu_begin_treenode(ctx, "Light Source")) {

        float intensity = lightSource->intensity();


        mu_layout_begin_column(ctx);
        mu_layout_row(ctx, 2, (int[]) {70, 150}, 0);
        mu_label(ctx, "Intensity"); mu_slider(ctx, &intensity, 0, 15);
        mu_layout_end_column(ctx);

        lightSource->setIntensity(intensity);

        static float color[3] = { static_cast<float>(lightSource->color().r()),
                                  static_cast<float>(lightSource->color().g()),
                                  static_cast<float>(lightSource->color().b())};
        mu_label(ctx, "Light Color");

        mu_layout_begin_column(ctx);
        mu_layout_row(ctx, 2, (int[]) { 46, -1 }, 0);
        mu_label(ctx, "Red:");   mu_slider(ctx, &color[0], 0, 255);
        mu_label(ctx, "Green:"); mu_slider(ctx, &color[1], 0, 255);
        mu_label(ctx, "Blue:");  mu_slider(ctx, &color[2], 0, 255);
        mu_layout_end_column(ctx);

        lightSource->setColor(Color(color[0], color[1], color[2]));


        auto spotLight = _selectedObject->getComponent<SpotLight>();
        if(spotLight) { // If this light is a spotlight
            float innerConeCos = spotLight->innerConeCos();
            float outerConeCos = spotLight->outerConeCos();

            mu_layout_begin_column(ctx);
            mu_layout_row(ctx, 2, (int[]) {100, 100}, 0);
            mu_label(ctx, "Inner Cone Cos"); mu_slider(ctx, &innerConeCos, outerConeCos, 1);
            mu_label(ctx, "Outer Cone Cos"); mu_slider(ctx, &outerConeCos, 0, 1);
            mu_layout_end_column(ctx);

            spotLight->setInnerConeCos(innerConeCos);
            spotLight->setOuterConeCos(outerConeCos);
        }

        mu_end_treenode(ctx);
    }
}

void WorldEditor::rigidObjectEditor() {
    auto ctx = _ctx.get();

    auto rigidObject = _selectedObject->getComponent<RigidObject>();
    if(!rigidObject) {
        return;
    }

    if (mu_begin_treenode(ctx, "Rigid Object")) {

        mu_label(ctx, ("Hitbox size:" + std::to_string(rigidObject->hitBoxSize()) +
        ". " + "In collision:" + std::to_string(rigidObject->inCollision())).c_str());

        bool hasCollision = rigidObject->hasCollision();
        mu_checkbox(ctx, "Has collision", &hasCollision);
        rigidObject->setCollision(hasCollision);

        mu_layout_begin_column(ctx);
        mu_layout_row(ctx, 2, (int[]) {100, 100}, 0);
        if (mu_button(ctx, "Velocity")) { _operationRigidObject = 0; }
        if (mu_button(ctx, "Acceleration")) { _operationRigidObject = 1; }
        mu_layout_end_column(ctx);

        switch (_operationRigidObject) {
            case 0:

                static float velocity[3] = {static_cast<float>(rigidObject->velocity().x()),
                                            static_cast<float>(rigidObject->velocity().y()),
                                            static_cast<float>(rigidObject->velocity().z())};

                mu_label(ctx, "Velocity");
                mu_layout_begin_column(ctx);
                mu_layout_row(ctx, 2, (int[]) { 30, -1 }, 0);
                mu_label(ctx, "Vx:");   mu_slider(ctx, &velocity[0], -50, 50);
                mu_label(ctx, "Vy:"); mu_slider(ctx, &velocity[1], -50, 50);
                mu_label(ctx, "Vz:");  mu_slider(ctx, &velocity[2], -50, 50);
                mu_layout_end_column(ctx);

                rigidObject->setVelocity(Vec3D(velocity[0], velocity[1], velocity[2]));
                break;
            case 1:
                static float acceleration[3] = {static_cast<float>(rigidObject->acceleration().x()),
                                                static_cast<float>(rigidObject->acceleration().y()),
                                                static_cast<float>(rigidObject->acceleration().z())};

                mu_label(ctx, "Acceleration");
                mu_layout_begin_column(ctx);
                mu_layout_row(ctx, 2, (int[]) { 30, -1 }, 0);
                mu_label(ctx, "Ax:");   mu_slider(ctx, &acceleration[0], -50, 50);
                mu_label(ctx, "Ay:"); mu_slider(ctx, &acceleration[1], -50, 50);
                mu_label(ctx, "Az:");  mu_slider(ctx, &acceleration[2], -50, 50);
                mu_layout_end_column(ctx);

                rigidObject->setAcceleration(Vec3D(acceleration[0], acceleration[1], acceleration[2]));

                break;
        }

        mu_end_treenode(ctx);
    }
}

void WorldEditor::objectEditor() {
    auto ctx = _ctx.get();

    if (_selectedObject && mu_begin_treenode(ctx, "Object editor")) {
        mu_text(ctx, ("Object name: " + _selectedObject->name().str()).c_str());

        transformMatrixEditor();
        triangleMeshEditor();
        lineMeshEditor();
        lightSourceEditor();
        rigidObjectEditor();

        mu_layout_row(ctx, 1, (int[]) {120}, 0);
        if (mu_button(ctx, "Delete object")) {
            _world->remove(_selectedObject->name());
            _selectedObject.reset();
        }

        mu_end_treenode(ctx);
    }
}

void WorldEditor::renderSettings() {
    auto ctx = _ctx.get();

    if (mu_begin_treenode(ctx, "Render Settings")) {

        mu_checkbox(ctx, "Transparent objects", &_enableTransparency);
        mu_checkbox(ctx, "Borders of triangles", &_enableTriangleBorders);
        mu_checkbox(ctx, "Texturing", &_enableTexturing);
        mu_checkbox(ctx, "Texture antialiasing (mipmapping)", &_enableMipmapping);
        mu_checkbox(ctx, "Depth test", &_enableDepthTest);

        if (mu_begin_treenode(ctx, "Lighting")) {
            mu_checkbox(ctx, "Lighting", &_enableLighting);
            mu_checkbox(ctx, "Exact lighting", &_enableTrueLighting);

            mu_layout_begin_column(ctx);
            mu_layout_row(ctx, 2, (int[]) { 70, 150 }, 0);
            mu_label(ctx, "LOD near");   mu_slider(ctx, &_lightingLODNearDistance, 0, 100);
            mu_label(ctx, "LOD far"); mu_slider(ctx, &_lightingLODFarDistance, _lightingLODNearDistance, _lightingLODNearDistance+100);
            mu_layout_end_column(ctx);

            mu_end_treenode(ctx);
        }

        mu_end_treenode(ctx);
    }
}

void WorldEditor::updateScreenSettings() {
    _screen->setLighting(_enableLighting);
    _screen->setTrueLighting(_enableTrueLighting);
    _screen->setTransparency(_enableTransparency);
    _screen->setTriangleBorders(_enableTriangleBorders);
    _screen->setTexturing(_enableTexturing);
    _screen->setMipmapping(_enableMipmapping);
    _screen->setDepthTest(_enableDepthTest);
    _screen->setLightingLODNearDistance(_lightingLODNearDistance);
    _screen->setLightingLODFarDistance(_lightingLODFarDistance);
}

void WorldEditor::updateControllers() {

    if(_isControllerActive) {
        _screen->drawStrokeRectangle(Consts::STANDARD_SCREEN_WIDTH/2-1, Consts::STANDARD_SCREEN_HEIGHT/2-7, 1, 14, Color::BLACK);
        _screen->drawStrokeRectangle(Consts::STANDARD_SCREEN_WIDTH/2-7, Consts::STANDARD_SCREEN_HEIGHT/2-1, 14, 1, Color::BLACK);
    }

    if(_isControllerActive) {
        if(_selectedObject) {
            _objController->update();
        } else {
            _cameraController->update();
        }
    }

#ifndef NDEBUG
    auto debugRayCast = _world->rayCast(_camera->transformMatrix()->position(), _camera->transformMatrix()->position() + _camera->transformMatrix()->lookAt(), {_redCube->name()});
    _objInFocus = debugRayCast.intersected;
    _redCube->getComponent<TriangleMesh>()->setVisible(_objInFocus);
    if(_objInFocus) {
        _redCube->getComponent<TransformMatrix>()->translateToPoint(debugRayCast.pointOfIntersection);
    }
#endif

    if(_isControllerActive) {
        // select object:
        if (Keyboard::isKeyTapped(SDLK_o) || Mouse::isButtonTapped(SDL_BUTTON_LEFT)) {
            auto rayCast = _world->rayCast(_camera->transformMatrix()->position(), _camera->transformMatrix()->position() + _camera->transformMatrix()->lookAt(), {_redCube->name()});

            if(rayCast.intersected) {
                _selectedObject = rayCast.triangleMesh->assignedToShared();

                _objController = std::make_shared<ObjectController>(_selectedObject->getComponent<TransformMatrix>());
                Log::log("Object " + rayCast.triangleMesh->assignedToPtr()->name().str() + " selected.");
            }
        }

        // deselect object:
        if (Keyboard::isKeyTapped(SDLK_p)) {
            _selectedObject.reset();
        }
    }

    if(Keyboard::isKeyTapped(SDLK_F1)) {
        _screen->makeScreenShot().save2png(FilePath("screenshot_" + Time::getLocalTimeInfo("%F_%H-%M-%S") + ".png"));
    }

    if(Keyboard::isKeyTapped(SDLK_F2)) {
        if(!_isRecording) {
            _screen->startRender();
        } else{
            _screen->stopRender();
        }
        _isRecording = !_isRecording;
    }

    if(_selectedObject && _selectedObject->getComponent<TriangleMesh>()) {
        _selectedObjectBounds->getComponent<LineMesh>()->setVisible(true);
        _selectedObjectBounds->getComponent<TransformMatrix>()->undoTransformations();
        auto Mbounds = _selectedObject->getComponent<TriangleMesh>()->bounds()*_selectedObject->getComponent<TransformMatrix>()->fullModel();
        _selectedObjectBounds->getComponent<TransformMatrix>()->scale(Mbounds.extents*2 + Vec3D::EPS());
        _selectedObjectBounds->getComponent<TransformMatrix>()->translate(Mbounds.center);
    } else {
        _selectedObjectBounds->getComponent<LineMesh>()->setVisible(false);
    }

    if(Keyboard::isKeyTapped(SDLK_q)) {
        if(!_isControllerActive && _selectedObject) {
            _objController = std::make_shared<ObjectController>(_selectedObject->getComponent<TransformMatrix>());
        }

        _isControllerActive = !_isControllerActive;

        SDL_SetRelativeMouseMode(static_cast<SDL_bool>(_isControllerActive));
        SDL_ShowCursor(!_isControllerActive);
    }

    if (_selectedObject && _isControllerActive) {
        // object scale x:
        if (Keyboard::isKeyPressed(SDLK_UP)) {
            _selectedObject->getComponent<TransformMatrix>()->scaleInside(Vec3D(1 + Time::deltaTime(), 1, 1));
        }
        // object scale y:
        if (Keyboard::isKeyPressed(SDLK_DOWN)) {
            _selectedObject->getComponent<TransformMatrix>()->scaleInside(Vec3D(1, 1 + Time::deltaTime(), 1));
        }
        // object scale z:
        if (Keyboard::isKeyPressed(SDLK_LEFT)) {
            _selectedObject->getComponent<TransformMatrix>()->scaleInside(Vec3D(1, 1, 1 + Time::deltaTime()));
        }

        // undo transformations
        if (Keyboard::isKeyPressed(SDLK_u)) {
            _selectedObject->getComponent<TransformMatrix>()->undoTransformations();
        }

        // delete object
        if (Keyboard::isKeyPressed(SDLK_DELETE)) {
            _world->remove(_selectedObject->name());
            _selectedObject.reset();
        }
    }
}

void WorldEditor::update() {

    if(!_isControllerActive) {
        handleInputEvents();
    }

    processFrame();

    updateScreenSettings();
    updateControllers();

    renderGui();
}
