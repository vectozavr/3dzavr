#include "Font.h"
#include <utils/Log.h>

bool FontTag::contains(const std::string& str) const {
    if(_name.find(str) != std::string::npos) {
        return true;
    }
    return false;
}

Font::Font(const FontTag& tag, const FilePath& fileName) : _tag(tag), _fileName(fileName) {

}

TTF_Font* Font::getFont(uint16_t fontSize) {

    if(_fonts.contains(fontSize) && _fonts[fontSize]) {
        return _fonts[fontSize];
    }

    TTF_Font* font = TTF_OpenFont(_fileName.str().c_str(), fontSize);
    // Confirm that it was loaded
    if(!font){
        Log::log("Font::getFont(): Could not load font " + _fileName.str());
        return nullptr;
    }
    _fonts.insert({fontSize,font});
    return font;
}


Font::~Font() {
    for(auto& f : _fonts) {
        if (f.second) {
            TTF_CloseFont(f.second);
        }
    }

    _fonts.clear();
}

std::pair<int, int> Font::getTextSize(const std::string &text, uint16_t fontSize) {
    TTF_Font* font = getFont(fontSize);
    if(!font) {
        return {0,0};
    }

    int w, h;
    TTF_SizeText(font, text.c_str(), &w, &h);

    return {w, h};
}
