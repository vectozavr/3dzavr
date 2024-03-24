
#include <io/Keyboard.h>
#include <io/Mouse.h>
#include <io/WorldEditorGui.h>
#include <utils/ResourceManager.h>

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
        res += f->getTextSize(std::string(1, *p), 12).first;;
    }
    return res;
}

int textHeight(mu_Font font) {
    return 15;
}

void WorldEditorGui::processFrame() {
    mu_begin(_ctx.get());

    controlPanel();

    mu_end(_ctx.get());
}

void WorldEditorGui::renderGui() {
    _setDepthTest(false);
    mu_Command *cmd = nullptr;
    while (mu_next_command(_ctx.get(), &cmd)) {
        switch (cmd->type) {
            case MU_COMMAND_TEXT:
                _drawText(cmd->text.str, cmd->text.pos.x, cmd->text.pos.y,
                          Color(cmd->text.color.r, cmd->text.color.g, cmd->text.color.b, cmd->text.color.a));

                break;
            case MU_COMMAND_RECT:
                _drawRectangle(cmd->rect.rect.x, cmd->rect.rect.y, cmd->rect.rect.w, cmd->rect.rect.h,
                               Color(cmd->rect.color.r, cmd->rect.color.g, cmd->rect.color.b, cmd->rect.color.a));
                break;
            case MU_COMMAND_ICON:
                drawUiIcon(cmd->icon.id, cmd->icon.rect.x, cmd->icon.rect.y);
                break;
        }
    }
    _setDepthTest(true);
}

void WorldEditorGui::handleInputEvents() {
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

void WorldEditorGui::update() {
    handleInputEvents();
    processFrame();
    renderGui();
}

WorldEditorGui::WorldEditorGui(const std::function<void(bool)> &setDepthTestFunction,
                               const std::function<void(int, int, int, int, const Color &, uint16_t)> &drawLineFunction,
                               const std::function<void(int, int, uint16_t, uint16_t, const Color &)> &drawRectangleFunction,
                               const std::function<void(const std::string &, int, int, const Color &)> &drawTextFunction,
                               const std::shared_ptr<Group>& objects) :
         _setDepthTest(setDepthTestFunction), _drawLine(drawLineFunction),
         _drawRectangle(drawRectangleFunction), _drawText(drawTextFunction), _objects(objects){
    _ctx = std::make_unique<mu_Context>();
    mu_init(_ctx.get());
    _ctx->text_width = textWidth;
    _ctx->text_height = textHeight;

    //_ctx.get()->style->colors[MU_COLOR_WINDOWBG] = {50, 50, 50, 200};
}

void WorldEditorGui::drawUiIcon(int id, uint16_t x, uint16_t y) {
    switch (id) {
        case MU_ICON_CLOSE:
            _drawLine(x+9, y+7, x+16, y+14, Color::WHITE_GRAY, 2);
            _drawLine(x+9, y+14, x+16, y+7, Color::WHITE_GRAY, 2);
            break;
        case MU_ICON_CHECK:
            _drawLine(x+7, y+14, x+4, y+11, Color::WHITE_GRAY, 2);
            _drawLine(x+7, y+14, x+15, y+6, Color::WHITE_GRAY, 2);
            break;
        case MU_ICON_COLLAPSED:
            _drawLine(x+7, y+14, x+12, y+10, Color::WHITE_GRAY, 2);
            _drawLine(x+7, y +6, x+12, y+10, Color::WHITE_GRAY, 2);
            break;
        case MU_ICON_EXPANDED:
            _drawLine(x+6, y+8, x+10, y+12, Color::WHITE_GRAY, 2);
            _drawLine(x+14, y+8, x+10, y+12, Color::WHITE_GRAY, 2);
            break;
        default:
            break;
    }
}

void WorldEditorGui::controlPanel() {
    auto ctx = _ctx.get();

    if (mu_begin_window(ctx, "Control Panel", mu_rect(Consts::STANDARD_SCREEN_WIDTH, 0,
                                                      Consts::STANDARD_EDITOR_WIDTH-Consts::STANDARD_SCREEN_WIDTH, Consts::STANDARD_SCREEN_HEIGHT))) {

        objectEditor();

        if (mu_begin_treenode(ctx, "World tree")) {
            groupTree(_objects);

            mu_end_treenode(ctx);
        }

        renderSettings();


        mu_layout_row(ctx, 1, (int[]) { 210 }, 20);
        mu_text(ctx, "Press 'q' to enter/quit free camera mode");
        mu_end_window(ctx);
    }
}

void WorldEditorGui::renderSettings() {
    auto ctx = _ctx.get();

    if (mu_begin_treenode(ctx, "Render Settings")) {
        mu_checkbox(ctx, "Lighting", &_enableLighting);
        mu_checkbox(ctx, "Exact lighting", &_enableTrueLighting);
        mu_checkbox(ctx, "Transparent objects", &_enableTransparency);
        mu_checkbox(ctx, "Borders of triangles", &_enableTriangleBorders);
        mu_checkbox(ctx, "Texturing", &_enableTexturing);
        mu_checkbox(ctx, "Texture antialiasing (mipmapping)", &_enableMipmapping);
        mu_checkbox(ctx, "Depth test", &_enableDepthTest);

        mu_end_treenode(ctx);
    }
}

void WorldEditorGui::groupTree(const std::shared_ptr<Group> &group) {
    auto ctx = _ctx.get();

    if (mu_begin_treenode(ctx, group->name().str().c_str())) {

        mu_layout_row(ctx, 1, (int[]) { 200 }, 0);
        if (group != _objects && mu_button_ex(ctx, "Select the whole group", MU_ICON_CHECK, 1)) {
            _selectedObject = group;
        }

        mu_text(ctx, "The content of the group");
        for(const auto& [objTag, obj] : *group) {
            std::shared_ptr<Group> subGroup = std::dynamic_pointer_cast<Group>(obj);
            if(subGroup) {
                // We need to recursively continue to draw subgroup
                groupTree(subGroup);
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

void WorldEditorGui::objectEditor() {
    auto ctx = _ctx.get();

    if (_selectedObject && mu_header_ex(ctx, "Object editor", MU_OPT_EXPANDED)) {
        mu_text(ctx, ("Object name: " + _selectedObject->name().str()).c_str());


        auto pos = _selectedObject->position();
        mu_text(ctx, ("X / Y / Z: " + std::to_string(pos.x()) + " / " + std::to_string(pos.y()) + " / " + std::to_string(pos.z()) ).c_str());


        mu_text(ctx, "Transform object:");

        mu_layout_row(ctx, 3, (int[]) { 60, 60, 60 }, 0);
        if (mu_button(ctx, "Translate")) { _operation = 0; }
        if (mu_button(ctx, "Scale")) { _operation = 1; }
        if (mu_button(ctx, "Rotate")) { _operation = 2; }

        float x, y, z;

        switch (_operation) {
            case 0:
                mu_layout_begin_column(ctx);
                mu_layout_row(ctx, 2, (int[]) { 70, 150 }, 0);
                mu_label(ctx, "Translate X:");   mu_slider(ctx, &x, -30*Time::deltaTime(), 30*Time::deltaTime());
                mu_label(ctx, "Translate Y:"); mu_slider(ctx, &y, -30*Time::deltaTime(), 30*Time::deltaTime());
                mu_label(ctx, "Translate Z:");  mu_slider(ctx, &z, -30*Time::deltaTime(), 30*Time::deltaTime());
                mu_layout_end_column(ctx);
                _selectedObject->translate(Vec3D(x, y, z));
                break;
            case 1:
                mu_layout_begin_column(ctx);
                mu_layout_row(ctx, 2, (int[]) { 70, 150 }, 0);
                mu_label(ctx, "Scale X:");   mu_slider(ctx, &x, -3*Time::deltaTime(), 3*Time::deltaTime());
                mu_label(ctx, "Scale Y:"); mu_slider(ctx, &y, -3*Time::deltaTime(), 3*Time::deltaTime());
                mu_label(ctx, "Scale Z:");  mu_slider(ctx, &z, -3*Time::deltaTime(), 3*Time::deltaTime());
                mu_layout_end_column(ctx);
                _selectedObject->scale(Vec3D(1 + x, 1 + y, 1 + z));
                break;
            case 2:
                mu_layout_begin_column(ctx);
                mu_layout_row(ctx, 2, (int[]) { 70, 150 }, 0);
                mu_label(ctx, "Rotate X:");   mu_slider(ctx, &x, -3*Time::deltaTime(), 3*Time::deltaTime());
                mu_label(ctx, "Rotate Y:"); mu_slider(ctx, &y, -3*Time::deltaTime(), 3*Time::deltaTime());
                mu_label(ctx, "Rotate Z:");  mu_slider(ctx, &z, -3*Time::deltaTime(), 3*Time::deltaTime());
                mu_layout_end_column(ctx);
                _selectedObject->rotate(Vec3D(x, y, z));
                break;
        }



        mu_layout_row(ctx, 2, (int[]) { 120, 120}, 0);
        if (mu_button(ctx, "Undo Transform")) {
            _selectedObject->transform(_selectedObject->invModel());
        }
        if (mu_button(ctx, "Delete object")) {
            _objects->remove(_selectedObject->name());
            _selectedObject = nullptr;
        }
    }
}

