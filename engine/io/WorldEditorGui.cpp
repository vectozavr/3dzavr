
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

    renderSettings();

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
                               const std::function<void(const std::string &, int, int, const Color &)> &drawTextFunction) :
         _setDepthTest(setDepthTestFunction), _drawLine(drawLineFunction),
         _drawRectangle(drawRectangleFunction), _drawText(drawTextFunction){
    _ctx = std::make_unique<mu_Context>();
    mu_init(_ctx.get());
    _ctx->text_width = textWidth;
    _ctx->text_height = textHeight;

    _ctx.get()->style->colors[MU_COLOR_WINDOWBG] = {50, 50, 50, 200};
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

void WorldEditorGui::renderSettings() {
    if (mu_begin_window(_ctx.get(), "Render Settings", mu_rect(Consts::STANDARD_SCREEN_WIDTH-240, 10, 230, 250))) {

        mu_checkbox(_ctx.get(), "Lighting", &_enableLighting);
        mu_checkbox(_ctx.get(), "Exact lighting", &_enableTrueLighting);
        mu_checkbox(_ctx.get(), "Transparent objects", &_enableTransparency);
        mu_checkbox(_ctx.get(), "Borders of triangles", &_enableTriangleBorders);
        mu_checkbox(_ctx.get(), "Texturing", &_enableTexturing);
        mu_checkbox(_ctx.get(), "Texture antialiasing (mipmapping)", &_enableMipmapping);
        mu_checkbox(_ctx.get(), "Depth test", &_enableDepthTest);

        mu_layout_row(_ctx.get(), 1, (int[]) { 210 }, 20);
        mu_text(_ctx.get(), "Press 'q' to enter/quit free camera mode");

        mu_end_window(_ctx.get());
    }
}
