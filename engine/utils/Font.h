#ifndef UTILS_FONT_H
#define UTILS_FONT_H

#include <string>
#include <map>

#include <utils/FilePath.h>

#include "SDL_ttf.h"

class FontTag final {
private:
    const std::string _name;
public:
    explicit FontTag(const std::string& name = "") : _name(name) {}

    FontTag(const FontTag& other) : _name(other._name) {}

    [[nodiscard]] std::string str() const { return _name; }
    [[nodiscard]] bool empty() const { return _name.empty(); }

    bool operator==(const FontTag &tag) const { return _name == tag._name; }
    bool operator!=(const FontTag &tag) const { return _name != tag._name; }
    bool operator<(const FontTag &tag) const { return _name < tag._name; }

    [[nodiscard]] bool contains(const std::string& str) const;
};

class Font {
private:
    const FontTag _tag;
    const FilePath _fileName;

    // Here we store this font for different fontsize
    std::map<uint16_t, TTF_Font*> _fonts;
public:
    Font(const FontTag& tag, const FilePath& fileName);

    TTF_Font* getFont(uint16_t fontSize=14);

    ~Font();
};


#endif //ENGINE_FONT_H
