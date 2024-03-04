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
    if(font == nullptr){
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