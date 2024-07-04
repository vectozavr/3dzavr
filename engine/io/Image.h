#ifndef IO_IMAGE_H
#define IO_IMAGE_H

#include <cstdint>
#include <png.h>

#include <components/props/Color.h>
#include <linalg/Vec2D.h>
#include <linalg/Vec3D.h>
#include <utils/FilePath.h>

/**
 * @class Image
 * @brief Represents an image, supporting operations like pixel manipulation and saving to PNG format.
 *
 * This class encapsulates the data and operations associated with an image, including its dimensions,
 * pixel data, and file operations. It supports basic image manipulation tasks such as setting and getting pixels,
 * downsampling, and saving the image to a PNG file. The class is designed to be non-copyable but movable to
 * efficiently handle image data.
 *
 * @note The class is designed to work with PNG images and uses libpng for PNG file operations.
 */
class Image {
public:
    /**
     * @enum CODE
     * @brief Represents the status codes for image operations.
     */
    enum CODE {SUCCESS, FILE_OPEN_ERROR, PNG_STRUCT_ERROR, INFO_STRUCT_ERROR, ERROR};

    /**
     * @enum CLAMP_MODE
     * @brief Defines the texture coordinate wrapping mode.
     */
    enum CLAMP_MODE {REPEAT, MIRRORED_REPEAT, CLAMP_TO_EDGE};

private:
    uint16_t _width; ///< The width of the image in pixels.
    uint16_t _height; ///< The height of the image in pixels.
    png_bytep _data = nullptr; ///< Pointer to the image's pixel data.
    bool _valid = false; ///< Indicates whether the image is in a valid state.
    FilePath _filename; ///< The file path of the image.

    /**
     * @brief Invalidates the image, marking it as not valid.
     */
    void invalidate();

public:
    /**
     * @brief Constructs an Image with specified dimensions.
     * @param width The width of the image in pixels. Defaults to standard screen width.
     * @param height The height of the image in pixels. Defaults to standard screen height.
     */
    explicit Image(uint16_t width = Consts::STANDARD_SCREEN_WIDTH, uint16_t height = Consts::STANDARD_SCREEN_HEIGHT);

    /**
     * @brief Constructs an Image by loading it from a file.
     * @param filename The path to the image file.
     */
    explicit Image(const FilePath &filename);

    /**
     * @brief Constructs an Image from a buffer of pixel data.
     * @param pixelBuffer A vector containing the pixel data.
     * @param width The width of the image in pixels.
     * @param height The height of the image in pixels.
     */
    Image(const std::vector<uint32_t>& pixelBuffer, uint16_t width, uint16_t height);

    // Copy operations are deleted to prevent copying of image data.
    Image(const Image& img) = delete;
    Image& operator=(const Image &img) = delete;

    /**
     * @brief Move constructor.
     * @param img The source image to move from.
     */
    Image(Image&& img) noexcept;

    /**
     * @brief Move assignment operator.
     *
     * Transfers the ownership of an image's resources from one image to another. This operation
     * leaves the moved-from image in a valid but unspecified state.
     *
     * @param img The source image to move from.
     * @return Reference to the current image.
     */
    Image& operator=(Image&& img) noexcept;

    /**
     * @brief Gets the width of the image.
     *
     * @return The width of the image in pixels.
     */
    [[nodiscard]] uint16_t width() const { return _width; };

    /**
     * @brief Gets the height of the image.
     *
     * @return The height of the image in pixels.
     */
    [[nodiscard]] uint16_t height() const { return _height; };

    /**
     * @brief Checks if the image is in a valid state.
     *
     * An image is considered valid if it has been successfully loaded or created and has not been moved from.
     *
     * @return True if the image is valid, false otherwise.
     */
    [[nodiscard]] bool isValid() const { return _valid; };

    /**
     * @brief Sets the color of a pixel at the specified coordinates.
     *
     * @param x The x-coordinate of the pixel.
     * @param y The y-coordinate of the pixel.
     * @param color The color to set the pixel to.
     */
    void set_pixel(uint16_t x, uint16_t y, const Color& color);

    /**
     * @brief Gets the color of a pixel at the specified coordinates.
     *
     * @param x The x-coordinate of the pixel.
     * @param y The y-coordinate of the pixel.
     * @return The color of the pixel at the specified coordinates.
     */
    [[nodiscard]] Color get_pixel_unsafe(uint16_t x, uint16_t y) const;

    /**
     * @brief Gets the color of a pixel at the specified coordinates.
     *
     * @param x The x-coordinate of the pixel.
     * @param y The y-coordinate of the pixel.
     * @return The color of the pixel at the specified coordinates.
     */
    [[nodiscard]] Color get_pixel(uint16_t x, uint16_t y) const;

    /**
     * @brief Gets the color of a pixel at the specified texture coordinates.
     *
     * @param uv The texture coordinates of the pixel.
     * @param mode The texture coordinate wrapping mode.
     * @param bottomUp Whether the texture is bottom-up.
     * @return The color of the pixel at the specified texture coordinates.
     */
    [[nodiscard]] Color get_pixel_from_UV(const Vec2D& uv, CLAMP_MODE mode = REPEAT, bool bottomUp = true) const;

    /**
     * @brief Creates a downsampled version of the current image.
     *
     * This method generates a new Image object that is a downsampled version of the current image,
     * effectively reducing its dimensions and detail for purposes such as mipmapping or performance optimization.
     * The specific downsampling algorithm and factor are determined by the implementation.
     *
     * @return A new Image object that is a downsampled version of this image.
     */
    [[nodiscard]] Image downSampled() const;

    /**
     * @brief Saves the image to a PNG file.
     *
     * This method saves the current image to a file in PNG format. It uses libpng to handle the PNG
     * file creation and writing process. The bit depth parameter allows specifying the color depth of
     * the saved image, which can affect the image's visual quality and file size.
     *
     * @param file_name The path to the file where the image should be saved.
     * @param bit_depth The bit depth to use for the saved image. Defaults to 8 bits per color channel.
     * @return A status code indicating the result of the save operation. Success or an error code.
     */
    CODE save2png(const FilePath& file_name, uint16_t bit_depth = 8);

    /**
     * @brief Gets a pointer to the image's raw pixel data.
     *
     * This method provides access to the raw pixel data of the image. This can be useful for direct
     * manipulation of pixels, analysis, or for use with APIs that require direct access to pixel data.
     * The data is stored in a format compatible with libpng.
     *
     * @return A pointer to the raw pixel data of the image.
     */
    [[nodiscard]] png_bytep data() const {return _data; }

    /**
     * @brief Gets the file name of the image.
     *
     * @return The file name of the image.
     */
    [[nodiscard]] FilePath fileName() const { return _filename; }

    /**
     * @brief Destructor.
     */
    ~Image();
};


#endif //IO_IMAGE_H
