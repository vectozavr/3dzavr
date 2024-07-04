#ifndef PROPS_TEXTURE_H
#define PROPS_TEXTURE_H

#include <vector>

#include "io/Image.h"
#include "utils/FilePath.h"

/**
 * @file Texture.h
 * @brief Defines the Texture class for handling texture images in rendering.
 *
 * This class manages texture images, including loading from files, accessing pixel data,
 * and handling transparency. It supports resampling for efficient use in rendering.
 */

class Texture {
private:
    /**
     * Stores resampled versions of the image for efficient rendering,
     * down to a 1x1 image representing the average color of the whole texture.
     */
    std::vector<Image> _texture;

    /// The file path of the texture image.
    FilePath _filename;

    /// Indicates whether the texture has any transparency.
    bool _isTransparent = false;

    /**
     * Checks the texture image for transparency and sets _isTransparent accordingly.
     */
    void checkTransparency();

    /**
     * Generates downsampled versions of the texture for efficient rendering.
     */
    void downSample();

public:
    /**
     * Constructs a Texture object from a file.
     * @param filename The path to the texture file.
     */
    explicit Texture(const FilePath& filename);

    /**
     * Constructs a Texture object directly from an Image object.
     * @param image The image to use as the texture.
     */
    explicit Texture(Image& image);

    /**
     * Retrieves the color of a pixel at specified coordinates.
     * @param x The x-coordinate of the pixel.
     * @param y The y-coordinate of the pixel.
     * @return The color of the specified pixel.
     */
    [[nodiscard]] Color get_pixel(uint16_t x, uint16_t y) const;

    /**
     * Retrieves the color of a pixel based on UV coordinates.
     * @param uv The UV coordinates of the pixel.
     * @return The color of the specified pixel.
     */
    [[nodiscard]] Color get_pixel_from_UV(const Vec2D& uv) const;

    /**
     * Retrieves a resampled image based on the specified area.
     * @param area The area for which to retrieve the resampled image.
     * @return A reference to the resampled Image object.
     */
    [[nodiscard]] const Image& get_sample(double area) const;

    /**
     * Retrieves the color of a pixel based on UV coordinates and a specific area for resampling.
     * @param uv The UV coordinates of the pixel.
     * @param area The area for which to retrieve the pixel color.
     * @return The color of the specified pixel.
     */
    [[nodiscard]] Color get_pixel_from_UV(const Vec2D& uv, double area) const;

    /**
     * Retrieves the width and height of the texture.
     * @return A Vec2D object containing the width and height of the texture.
     */
    [[nodiscard]] Vec2D size() const { return Vec2D(width(), height()); }

    /**
     * Retrieves the width of the texture.
     * @return The width of the texture.
     */
    [[nodiscard]] uint16_t width() const { return _texture.front().width(); }

    /**
     * Retrieves the height of the texture.
     * @return The height of the texture.
     */
    [[nodiscard]] uint16_t height() const { return _texture.front().height(); }

    /**
     * Checks whether the texture has any transparency.
     * @return True if the texture has transparency, false otherwise.
     */
    [[nodiscard]] bool isTransparent() const {return _isTransparent; }

    /**
     * Retrieves the file name of the texture.
     * @return The file name of the texture.
     */
    [[nodiscard]] FilePath fileName() const { return _filename; }
};


#endif //PROPS_TEXTURE_H
