#ifndef UTILS_FONT_H
#define UTILS_FONT_H

#include <string>
#include <map>

#include <utils/FilePath.h>

#include "SDL_ttf.h"

/**
 * @class FontTag
 * @brief Represents a tag for identifying fonts.
 *
 * This class encapsulates a tag used to identify fonts. It provides functionality to compare,
 * check emptiness, and perform containment checks on the tag's name.
 */
class FontTag final {
private:
    const std::string _name; ///< The name of the font tag.

public:
    /**
     * @brief Constructs a FontTag with an optional name.
     * @param name The name of the font tag. Defaults to an empty string.
     */
    explicit FontTag(const std::string& name = "") : _name(name) {}

    /**
     * @brief Copy constructor.
     * @param other The other FontTag to copy from.
     */
    FontTag(const FontTag& other) : _name(other._name) {}

    /**
     * @brief Returns the tag as a string.
     * @return The name of the font tag.
     */
    [[nodiscard]] std::string str() const { return _name; }

    /**
     * @brief Checks if the tag is empty.
     * @return True if the tag is empty, false otherwise.
     */
    [[nodiscard]] bool empty() const { return _name.empty(); }

    // Comparison operators
    bool operator==(const FontTag &tag) const { return _name == tag._name; }
    bool operator!=(const FontTag &tag) const { return _name != tag._name; }
    bool operator<(const FontTag &tag) const { return _name < tag._name; }

    /**
     * @brief Checks if the tag contains a specified string.
     * @param str The string to check for containment.
     * @return True if the tag contains the string, false otherwise.
     */
    [[nodiscard]] bool contains(const std::string& str) const;
};

/**
 * @class Font
 * @brief Manages font resources.
 *
 * This class is responsible for managing font resources, including loading fonts from files
 * and retrieving font instances at different sizes.
 */
class Font {
private:
    const FontTag _tag; ///< The tag identifying this font.
    const FilePath _fileName; ///< The file path to the font file.

    // Stores font instances by size for efficient reuse.
    std::map<uint16_t, TTF_Font*> _fonts;
public:
    /**
     * @brief Constructs a Font resource.
     * @param tag The tag identifying the font.
     * @param fileName The file path to the font file.
     */
    Font(const FontTag& tag, const FilePath& fileName);

    /**
     * @brief Retrieves a font instance at the specified size.
     * @param fontSize The desired font size.
     * @return A pointer to the TTF_Font instance.
     */
    TTF_Font* getFont(uint16_t fontSize=14);

    /**
     * @brief Calculates the size of rendered text.
     * @param text The text to measure.
     * @param fontSize The font size to use for measurement.
     * @return A pair of integers representing the width and height of the rendered text.
     */
    [[nodiscard]] std::pair<int, int> getTextSize(const std::string& text, uint16_t fontSize=14);

    /**
     * @brief Destructor, cleans up loaded font resources.
     */
    ~Font();
};


#endif //ENGINE_FONT_H
