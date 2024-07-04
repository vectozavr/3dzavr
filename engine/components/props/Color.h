#ifndef PROPS_COLOR_H
#define PROPS_COLOR_H

#include <array>
#include <cstdint>

/**
 * @file Color.h
 * @brief Defines the Color class, which represents RGBA colors.
 *
 * This class encapsulates colors in RGBA format, allowing for easy manipulation
 * and use within graphical applications. It supports various constructors for
 * creating colors from different types of inputs, such as individual RGBA values,
 * a single uint32_t value, or another Color object. Additionally, it provides
 * methods for accessing and modifying the color components, comparing colors,
 * and performing color operations like blending.
 */
class Color final {
private:
    std::array<uint8_t, 4> _c; ///< The color components stored as an array of four uint8_t values representing RGBA.

public:
    /**
     * @brief Copy constructor. Creates a Color object by copying the color components from another Color object.
     * @param other The Color object from which to copy the color components.
     */
    Color(const Color &other) {
        std::copy(other._c.begin(), other._c.end(), _c.begin());
    }

    /**
     * @brief Constructs a Color object from a single uint32_t value representing RGBA components.
     * @param c The uint32_t value where each byte represents a color component in the order RGBA.
     */
    explicit Color(uint32_t c) : _c({
        static_cast<unsigned char>((c >> 24) & 0xFF),
        static_cast<unsigned char>((c >> 16) & 0xFF),
        static_cast<unsigned char>((c >> 8) & 0xFF),
        static_cast<unsigned char>(c & 0xFF)
    }) {}

    /**
     * @brief Constructs a Color object from individual RGBA components.
     * @param r The red component of the color.
     * @param g The green component of the color.
     * @param b The blue component of the color.
     * @param a The alpha (transparency) component of the color. Defaults to 255 (opaque).
     */
    explicit Color(uint8_t r = 0, uint8_t g = 0, uint8_t b = 0, uint8_t a = 255) : _c({r, g, b, a}) {}

    /**
     * @brief Constructs a Color object from an array of RGBA components.
     * @param color An array of four uint8_t values representing the RGBA components of the color.
     */
    explicit Color(const std::array<uint8_t, 4> &color) : _c(color) {}

    /**
    * @brief Default assignment operator.
    * @param color The Color object to assign from.
    * @return A reference to the modified Color object.
    */
    Color &operator=(const Color &color) = default;

    /**
     * @brief Gets the red component of the color.
     *
     * @return uint8_t The red component as an unsigned 8-bit integer.
     */
    [[nodiscard]] inline uint8_t r() const { return _c[0]; }

    /**
     * @brief Gets the green component of the color.
     *
     * @return uint8_t The green component as an unsigned 8-bit integer.
     */
    [[nodiscard]] inline uint8_t g() const { return _c[1]; }

    /**
     * @brief Gets the blue component of the color.
     *
     * @return uint8_t The blue component as an unsigned 8-bit integer.
     */
    [[nodiscard]] inline uint8_t b() const { return _c[2]; }

    /**
     * @brief Gets the alpha (transparency) component of the color.
     *
     * @return uint8_t The alpha component as an unsigned 8-bit integer.
     */
    [[nodiscard]] inline uint8_t a() const { return _c[3]; }

    /**
     * @brief Accesses a specific color component by index.
     *
     * @param i The index of the component (0 for red, 1 for green, 2 for blue, 3 for alpha).
     * @return uint8_t& A reference to the specified component.
     */
    [[nodiscard]] inline uint8_t& operator[](std::size_t i) { return _c[i]; }

    /**
     * @brief Accesses a specific color component by index (const version).
     *
     * @param i The index of the component (0 for red, 1 for green, 2 for blue, 3 for alpha).
     * @return const uint8_t& A const reference to the specified component.
     */
    [[nodiscard]] inline const uint8_t& operator[](std::size_t i) const { return _c[i]; }

    /**
     * @brief Gets the color as a single uint32_t value representing RGBA components.
     *
     * @return uint32_t The color as a uint32_t with each byte representing a color component in the order RGBA.
     */
    [[nodiscard]] inline uint32_t rgba() const { return (_c[0] << 24) | (_c[1] << 16) | (_c[2] << 8) | _c[3]; }

    /**
     * @brief Checks if two colors are equal.
     *
     * @param other The color to compare with.
     * @return true if the colors are the same, false otherwise.
     */
    [[nodiscard]] inline bool operator==(const Color &other) const { return _c == other._c; }

    /**
     * @brief Checks if two colors are not equal.
     *
     * @param other The color to compare with.
     * @return true if the colors are not the same, false otherwise.
     */
    [[nodiscard]] inline bool operator!=(const Color &other) const { return !(*this == other); }

    /**
     * @brief Multiplies the color by a scalar alpha value.
     *
     * This operator allows for adjusting the intensity of the color's components by multiplying them
     * by a given alpha value. It can be used to apply transparency or darken/lighten the color.
     *
     * @param alpha The scalar value to multiply each color component by.
     * @return Color The resulting color after applying the multiplication.
     */
    [[nodiscard]] inline Color operator*(double alpha) const { return Color((uint8_t)(_c[0]*alpha),
                                                                            (uint8_t)(_c[1]*alpha),
                                                                            (uint8_t)(_c[2]*alpha),
                                                                            (uint8_t)(_c[3]*alpha)); };

    /**
     * @brief Adds the components of another color to this color.
     *
     * This operator allows for the combination of two colors by adding their respective RGBA components.
     * It can be used to blend colors by direct addition of their components.
     *
     * @param other The color to add to this color.
     * @return Color The resulting color after the addition.
     */
    [[nodiscard]] inline Color operator+(const Color &other) const { return Color(r()+other.r(),
                                                                                  g()+other.g(),
                                                                                  b()+other.b(),
                                                                                  a()+other.a()); };

    /**
     * @brief Blends this color with another color using the default SDL blend mode.
     *
     * This method blends two colors by first multiplying each RGB component of the source color
     * by its alpha value, then adding it to the RGB component of the destination color multiplied
     * by the inverse of the source alpha. The alpha component of the resulting color is calculated
     * by adding the source alpha to the destination alpha multiplied by the inverse of the source alpha.
     * This blend mode is commonly used in SDL (Simple DirectMedia Layer) for blending textures.
     *
     * @param other The other color to blend with this color.
     * @return Color The resulting blended color.
     */
    [[nodiscard]] inline Color blend(const Color& other) const {
        // default SDL blend mode:
        // dstRGB = (srcRGB * srcA) + (dstRGB * (1-srcA))
        // dstA = srcA + (dstA * (1-srcA))
        uint8_t ra = 255 - a();
        return Color(
            (static_cast<uint16_t>(r()) * a() + other.r() * ra) / 255,
            (static_cast<uint16_t>(g()) * a() + other.g() * ra) / 255,
            (static_cast<uint16_t>(b()) * a() + other.b() * ra) / 255,
            (static_cast<uint16_t>(a()) * 255 + other.a() * ra) / 255
        );
    }

    static const Color WHITE;
    static const Color BLACK;
    static const Color GRAY;
    static const Color WHITE_GRAY;
    static const Color RED;
    static const Color GREEN;
    static const Color BLUE;
    static const Color YELLOW;
    static const Color LIGHT_YELLOW;
    static const Color LIGHT_ORANGE;
};

#endif //PROPS_COLOR_H
