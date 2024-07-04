#ifndef IO_SCREEN_H
#define IO_SCREEN_H

#include <string>
#include <map>

#include "SDL.h"

#include <Consts.h>
#include <utils/Font.h>
#include <utils/Time.h>
#include <objects/Camera.h>
#include <components/geometry/Triangle.h>
#include <components/geometry/TriangleMesh.h>
#include <components/lighting/LightSource.h>


/**
 * @file Screen.h
 * @brief Defines the Screen class for rendering graphics using SDL.
 *
 * This class encapsulates SDL rendering functionalities, including window management, drawing primitives,
 * handling depth buffering, and managing video recording capabilities. It provides methods to draw various
 * geometric shapes, manage the window state, and configure rendering options such as lighting and texturing.
 */

class Screen final {
private:
    SDL_Renderer* _renderer = nullptr; ///< Pointer to the SDL renderer.
    SDL_Window* _window = nullptr; ///< Pointer to the SDL window.
    SDL_Texture* _screenTexture = nullptr; ///< Texture used for rendering to the screen.

    std::vector<double> _depthBuffer; ///< Buffer for depth testing to implement z-buffering.
    std::vector<uint32_t> _pixelBuffer; ///< Buffer for pixel data before it's transferred to the texture.

    uint16_t _width; ///< Width of the screen in pixels.
    uint16_t _height; ///< Height of the screen in pixels.
    bool _depthTest = false; ///< Flag to enable or disable depth testing.

    bool _renderVideo = false; ///< Flag to enable or disable video rendering.
    double _lastFrameTime = 0; ///< Time at which the last frame was rendered.
    int _clipFps = 30; ///< Frames per second for video rendering.
    int _clipCrf = 28; ///< Constant Rate Factor for video quality.
    FILE* _ffmpeg = nullptr; ///< File pointer for ffmpeg process used in video rendering.

    std::string _title = Consts::BUILD_INFO; ///< Title of the window.

    Color _background; ///< Background color of the screen.

    bool _isOpen = false; ///< Flag to check if the window is open.

    bool _enableLighting = true; ///< Flag to enable or disable lighting.
    bool _enableTrueLighting = false; ///< Flag to enable or disable more complex lighting calculations.
    bool _enableTransparency = true; ///< Flag to enable or disable transparency.
    bool _enableTriangleBorders = false; ///< Flag to enable or disable drawing borders around triangles.
    bool _enableTexturing = true; ///< Flag to enable or disable texturing.
    bool _enableMipmapping = true; ///< Flag to enable or disable mipmapping for textures.

    double _lightingLODNearDistance = Consts::LIGHTING_LOD_NEAR_DISTANCE; ///< Near distance for Level of Detail in lighting calculations.
    double _lightingLODFarDistance = Consts::LIGHTING_LOD_FAR_DISTANCE; ///< Far distance for Level of Detail in lighting calculations.


    /**
     * @brief Checks if the given depth is less than the stored depth at the specified pixel.
     *
     * This method is used for depth testing in 3D rendering. It compares the given depth value (z) with the
     * depth value stored in the depth buffer at the coordinates (x, y). It's a crucial part of implementing
     * z-buffering for hidden surface removal.
     *
     * @param x The x-coordinate of the pixel.
     * @param y The y-coordinate of the pixel.
     * @param z The depth value to compare.
     * @return true If the given depth is less than the stored depth, false otherwise.
     */
    [[nodiscard]] bool checkPixelDepth(uint16_t x, uint16_t y, double z) const;

    /**
     * @brief Draws a pixel on the screen at the specified coordinates without using the depth buffer and without checks of being within the screen bounds.
     *
     * This method directly plots a pixel on the screen at the given coordinates (x, y) with the specified color.
     * It bypasses any depth testing and does not check if the pixel is within the screen bounds.
     *
     * @param x The x-coordinate of the pixel.
     * @param y The y-coordinate of the pixel.
     * @param color The color of the pixel.
     */
    void drawPixelUnsafe(uint16_t x, uint16_t y, const Color& color);

    /**
     * @brief Draws a pixel on the screen at the specified coordinates with depth information without checks of being within the screen bounds.
     *
     * Similar to drawPixelUnsafe, but this version also takes a depth value (z) and plots the pixel only if it
     * passes the depth test. It does not perform bounds checking.
     *
     * @param x The x-coordinate of the pixel.
     * @param y The y-coordinate of the pixel.
     * @param z The depth value of the pixel.
     * @param color The color of the pixel.
     */
    void drawPixelUnsafe(uint16_t x, uint16_t y, double z, const Color &color); // With using depth buffer without checks

    // Helping functions for drawLine method
    void plotLineLow(int x_from, int y_from, int x_to, int y_to, const Color &color, uint16_t thickness);
    void plotLineHigh(int x_from, int y_from, int x_to, int y_to, const Color &color, uint16_t thickness);
    // Helping functions for drawLine method that take into account the thickness of the line
    void plotLineLow(const Vec3D& from, const Vec3D& to, const Color &color, uint16_t thickness);
    void plotLineHigh(const Vec3D& from, const Vec3D& to, const Color &color, uint16_t thickness);

    /**
     * @brief Draws a line on the screen between two points.
     *
     * This method draws a straight line from the starting point (from) to the ending point (to) with the specified color and thickness.
     * It uses the Bresenham's line algorithm for efficient rendering. The thickness parameter allows for drawing lines thicker than one pixel
     * by drawing multiple parallel lines. This method is suitable for drawing outlines, shapes, or simple paths.
     *
     * @param from The starting point of the line as a Vec2D object.
     * @param to The ending point of the line as a Vec2D object.
     * @param color The color of the line as a Color object.
     * @param thickness The thickness of the line in pixels. Defaults to 1 for a single-pixel-wide line.
     */
    void drawLine(const Vec2D& from, const Vec2D& to, const Color &color, uint16_t thickness = 1);

public:
    Screen& operator=(const Screen& scr) = delete;

    /**
     * @brief Opens a new window with specified dimensions and background color.
     *
     * This method initializes the SDL window and renderer with the given screen width, height, and background color.
     * It sets up the necessary environment for rendering graphics. The screen dimensions and background color can be
     * customized through the parameters, with defaults provided via constants.
     *
     * @param screenWidth The width of the screen in pixels. Defaults to standard screen width defined in Consts.
     * @param screenHeight The height of the screen in pixels. Defaults to standard screen height defined in Consts.
     * @param background The background color of the screen. Defaults to the background color defined in Consts.
     */
    void open(uint16_t screenWidth = Consts::STANDARD_SCREEN_WIDTH,
              uint16_t screenHeight = Consts::STANDARD_SCREEN_HEIGHT,
              const Color& background = Consts::BACKGROUND_COLOR);

    /**
     * @brief Displays the rendered content on the screen.
     *
     * This method updates the screen with the content rendered to the SDL renderer's target texture.
     * It should be called at the end of each frame to present the rendered graphics to the user.
     */
    void display();

    /**
     * @brief Clears the screen and depth buffer.
     *
     * This method clears the screen with the background color and resets the depth buffer. It is typically called
     * at the beginning of each frame to prepare for new rendering operations.
     */
    void clear();

    /**
     * @brief Draws a pixel on the screen at specified coordinates without depth testing.
     *
     * This method plots a single pixel on the screen at the given (x, y) coordinates with the specified color.
     * It bypasses the depth buffer, ensuring the pixel is drawn regardless of its depth value.
     *
     * @param x The x-coordinate of the pixel.
     * @param y The y-coordinate of the pixel.
     * @param color The color of the pixel.
     */
    void drawPixel(int x, int y, const Color& color);

    /**
     * @brief Draws a pixel on the screen at specified coordinates with depth testing.
     *
     * Similar to drawPixel, but this version also considers the depth value (z) of the pixel. The pixel is only drawn
     * if its depth is less than the current depth stored at its coordinates in the depth buffer.
     *
     * @param x The x-coordinate of the pixel.
     * @param y The y-coordinate of the pixel.
     * @param z The depth value of the pixel.
     * @param color The color of the pixel.
     */
    void drawPixel(int x, int y, double z, const Color& color);

    /**
     * @brief Draws a line on the screen between two specified points.
     *
     * This method draws a straight line from the starting point (x_from, y_from) to the ending point (x_to, y_to)
     * with the specified color and thickness. The thickness parameter allows for drawing lines thicker than one pixel.
     * This method is suitable for drawing outlines, shapes, or simple paths.
     *
     * @param x_from The x-coordinate of the starting point.
     * @param y_from The y-coordinate of the starting point.
     * @param x_to The x-coordinate of the ending point.
     * @param y_to The y-coordinate of the ending point.
     * @param color The color of the line.
     * @param thickness The thickness of the line in pixels. Defaults to 1 for a single-pixel-wide line.
     */
    void drawLine(int x_from, int y_from, int x_to, int y_to, const Color &color, uint16_t thickness = 1);

    /**
     * @brief Draws a line on the screen based on a Line object.
     *
     * This overload of the drawLine method takes a Line object, which encapsulates the starting and ending points,
     * and draws it with the specified color and thickness. It provides a convenient way to draw lines when the points
     * are already encapsulated in a Line object.
     *
     * @param line The Line object representing the line to be drawn.
     * @param color The color of the line.
     * @param thickness The thickness of the line in pixels. Defaults to 1 for a single-pixel-wide line.
     */
    void drawLine(const Line& line, const Color &color, uint16_t thickness = 1);

    /**
     * @brief Draws a triangle on the screen.
     *
     * This method draws a triangle defined by the Triangle object. It can optionally use a Material object
     * for texturing. If no material is provided, the triangle is drawn using a default color.
     *
     * @param triangle The Triangle object representing the triangle to be drawn.
     * @param material Optional. The Material object to use for rendering the triangle. Defaults to nullptr.
     */
    void drawTriangle(const Triangle &triangle, Material* material = nullptr);

    /**
     * @brief Draws a triangle on the screen with a specified color.
     *
     * This overload of the drawTriangle method draws the triangle defined by the Triangle object using a solid color.
     * This method is suitable for simple rendering needs where advanced effects are not required.
     *
     * @param triangle The Triangle object representing the triangle to be drawn.
     * @param color The color to use for the triangle.
     */
    void drawTriangle(const Triangle &triangle, const Color &color);

    /**
     * @brief Draws a filled rectangle on the screen.
     *
     * This method draws a filled rectangle at the specified (x, y) coordinates with the given width and height,
     * using a solid color. It is useful for drawing backgrounds, UI elements, or any rectangular shapes.
     *
     * @param x The x-coordinate of the rectangle's top-left corner.
     * @param y The y-coordinate of the rectangle's top-left corner.
     * @param width The width of the rectangle.
     * @param height The height of the rectangle.
     * @param color The color used to fill the rectangle.
     */
    void drawRectangle(int x, int y, uint16_t width, uint16_t height, const Color &color);

    /**
     * @brief Draws a filled rectangle on the screen with a specified material.
     *
     * This overload of the drawRectangle method draws a filled rectangle at the specified (x, y) coordinates with the given
     * width and height, using a Material object for texturing. It is useful for rendering complex shapes with textures.
     *
     * @param x The x-coordinate of the rectangle's top-left corner.
     * @param y The y-coordinate of the rectangle's top-left corner.
     * @param width The width of the rectangle.
     * @param height The height of the rectangle.
     * @param material The Material object to use for texturing the rectangle.
     */
    void drawRectangle(int x, int y, uint16_t width, uint16_t height, Material* material = nullptr);

    /**
     * @brief Draws a filled circle on the screen.
     *
     * This method draws a filled circle centered at the specified (x, y) coordinates with the given radius,
     * using a solid color. It is useful for drawing circular shapes or markers.
     *
     * @param x The x-coordinate of the circle's center.
     * @param y The y-coordinate of the circle's center.
     * @param r The radius of the circle.
     * @param fillColor The color used to fill the circle.
     */
    void drawCircle(int x, int y, uint16_t r, const Color &fillColor);

    /**
     * @brief Draws a filled circle on the screen with depth testing.
     *
     * This method draws a filled circle centered at the specified (x, y) coordinates with the given radius and depth (z),
     * using a solid color. It allows the circle to be correctly rendered in a 3D context by considering its depth.
     *
     * @param x The x-coordinate of the circle's center.
     * @param y The y-coordinate of the circle's center.
     * @param z The depth value of the circle, used for depth testing.
     * @param r The radius of the circle.
     * @param fillColor The color used to fill the circle.
     */
    void drawCircle(int x, int y, double z, uint16_t r, const Color &fillColor);

    /**
     * @brief Draws a rectangle with a stroke on the screen.
     *
     * This method draws a rectangle at the specified (x, y) coordinates with the given width and height,
     * filled with the specified color. It also draws a border around the rectangle with the specified stroke color
     * and thickness. This method is useful for drawing UI elements or highlighting areas on the screen.
     *
     * @param x The x-coordinate of the rectangle's top-left corner.
     * @param y The y-coordinate of the rectangle's top-left corner.
     * @param width The width of the rectangle.
     * @param height The height of the rectangle.
     * @param color The fill color of the rectangle.
     * @param thickness The thickness of the stroke. Defaults to 1 pixel.
     * @param strokeColor The color of the stroke. Defaults to black.
     */
    void drawStrokeRectangle(int x, int y, uint16_t width, uint16_t height,
                             const Color &color, uint16_t thickness = 1, const Color &strokeColor = Color::BLACK);

    /**
     * @brief Draws text on the screen at the specified location.
     *
     * This method renders the given text string at the specified (x, y) coordinates with the specified color and font size.
     * If a font is provided, it will be used; otherwise, a default font will be selected. This method is useful for displaying
     * information, labels, or any other textual content on the screen.
     *
     * @param text The text string to be drawn.
     * @param x The x-coordinate where the text will start.
     * @param y The y-coordinate where the text will start.
     * @param color The color of the text. Defaults to black.
     * @param fontsize The size of the font. Defaults to 12 pixels.
     * @param font Optional. The font to use for rendering the text. Defaults to nullptr, which selects a default font.
     */
    void drawText(const std::string& text, int x, int y, const Color& color = Color::BLACK, uint16_t fontsize = 12, const std::shared_ptr<Font>& font = nullptr);

    /**
     * @brief Draws an image on the screen at the specified location.
     *
     * This method renders an image at the specified (x, y) coordinates. The image is provided as a shared pointer
     * to an Image object, which contains the pixel data and dimensions of the image. This method is useful for rendering
     * sprites, textures, or any other image-based content on the screen.
     *
     * @param x The x-coordinate where the image will be placed.
     * @param y The y-coordinate where the image will be placed.
     * @param img The shared pointer to the Image object to be drawn.
     */
    void drawImage(int x, int y, std::shared_ptr<Image> img);

    /**
     * @brief Draws a plot on the screen based on the provided data points.
     *
     * This method renders a plot on the screen at the specified location and size. The plot is drawn using
     * a series of data points, where each point is represented as a pair of double values. The first value
     * in the pair represents the x-coordinate, and the second value represents the y-coordinate of the point.
     * The plot is scaled to fit within the specified width and height.
     *
     * @param data A vector of pairs, where each pair contains the x and y coordinates of a data point.
     * @param x The x-coordinate of the top-left corner of the plot area.
     * @param y The y-coordinate of the top-left corner of the plot area.
     * @param w The width of the plot area.
     * @param h The height of the plot area.
     */
    void drawPlot(const std::vector<std::pair<double, double>>& data, int x, int y, uint16_t w, uint16_t h);

    /**
     * @brief Draws a triangle with lighting effects based on the provided light sources and camera position.
     *
     * This method renders a triangle with lighting effects, taking into account the positions and properties
     * of multiple light sources and the camera's position. The triangle is defined by two versions: one representing
     * its projected form on the screen (projectedTriangle) and the other representing its model in the 3D world (Mtriangle).
     * Lighting calculations are performed based on the 3D model of the triangle, the light sources, and the camera position.
     * A material can be specified for texturing the triangle; if no material is provided, a default color is used.
     *
     * @param projectedTriangle The Triangle object representing the triangle's projected form on the screen.
     * @param Mtriangle The Triangle object representing the triangle's model in the 3D world.
     * @param lights A vector of shared pointers to LightSource objects representing the light sources affecting the triangle.
     * @param cameraPosition The position of the camera in the 3D world, used for lighting calculations.
     * @param material Optional. The Material object to use for rendering the triangle. Defaults to nullptr, which uses a default color.
     */
    void drawTriangleWithLighting(const Triangle &projectedTriangle, const Triangle &Mtriangle,
                                  const std::vector<std::shared_ptr<LightSource>>& lights,
                                  const Vec3D& cameraPosition, Material* material = nullptr);

    /**
     * @brief Draws a triangle with lighting effects using a specified color.
     *
     * This overload of the drawTriangleWithLighting method renders the triangle with lighting effects, similar to the
     * previous method, but uses a specified color instead of a material for rendering. This is useful when the triangle
     * does not require texturing but still needs to be affected by lighting conditions.
     *
     * @param projectedTriangle The Triangle object representing the triangle's projected form on the screen.
     * @param Mtriangle The Triangle object representing the triangle's model in the 3D world.
     * @param lights A vector of shared pointers to LightSource objects representing the light sources affecting the triangle.
     * @param cameraPosition The position of the camera in the 3D world, used for lighting calculations.
     * @param color The color to use for rendering the triangle.
     */
    void drawTriangleWithLighting(const Triangle &projectedTriangle, const Triangle &Mtriangle,
                                  const std::vector<std::shared_ptr<LightSource>>& lights,
                                  const Vec3D& cameraPosition, const Color &color);

    /**
     * @brief Sets the window title.
     *
     * This method updates the title of the window to the specified string. It can be used to dynamically change
     * the window title during runtime, such as to reflect the current state of the application or game.
     *
     * @param title The new title for the window.
     */
    void setTitle(const std::string &title);

    /**
     * @brief Enables or disables depth testing.
     *
     * This method controls whether depth testing is enabled for rendering. Depth testing is used to determine
     * if an object is behind another object and thus should not be visible. Enabling depth testing can help
     * achieve more realistic 3D rendering by correctly handling overlapping objects.
     *
     * @param enable True to enable depth testing, false to disable it.
     */
    void setDepthTest(bool enable) { _depthTest = enable; };

    /**
     * @brief Enables or disables lighting effects.
     *
     * This method toggles the rendering of lighting effects. When enabled, objects can be affected by light sources,
     * which can add realism to the scene. Disabling lighting can be useful for performance optimization or for achieving
     * a specific visual style.
     *
     * @param enable True to enable lighting, false to disable it.
     */
    void setLighting(bool enable) { _enableLighting = enable;}

    /**
     * @brief Enables or disables more complex lighting calculations.
     *
     * This method toggles the use of more complex lighting calculations, which can provide more realistic lighting effects
     * at the cost of performance. This is useful for applications that require high-quality lighting.
     *
     * @param enable True to enable complex lighting calculations, false to disable them.
     */
    void setTrueLighting(bool enable) { _enableTrueLighting = enable; }

    /**
     * @brief Enables or disables transparency in rendering.
     *
     * This method controls whether transparency is considered when rendering objects. Enabling transparency allows for
     * rendering semi-transparent objects, which can be useful for effects like glass, water, or ghosting.
     *
     * @param enable True to enable transparency, false to disable it.
     */
    void setTransparency(bool enable) { _enableTransparency = enable; }

    /**
     * @brief Enables or disables drawing borders around triangles.
     *
     * This method toggles the rendering of borders around triangles. Enabling this can be useful for debugging or for
     * achieving a specific visual style, such as a wireframe view of 3D models.
     *
     * @param enable True to draw triangle borders, false to not draw them.
     */
    void setTriangleBorders(bool enable) { _enableTriangleBorders = enable; }

    /**
     * @brief Enables or disables texturing.
     *
     * This method controls whether textures are applied to objects during rendering. Texturing can add significant
     * detail to objects but may impact performance. Disabling texturing can be useful for performance optimization
     * or for achieving a flat color style.
     *
     * @param enable True to enable texturing, false to disable it.
     */
    void setTexturing(bool enable) { _enableTexturing = enable; }

    /**
     * @brief Enables or disables mipmapping for textures.
     *
     * Mipmapping improves texture quality and performance by using lower resolution textures for objects that are far away.
     * Enabling mipmapping can enhance visual quality, especially on textured surfaces that span large distances.
     *
     * @param enable True to enable mipmapping, false to disable it.
     */
    void setMipmapping(bool enable) { _enableMipmapping = enable; }

    /**
     * @brief Sets the near distance for Level of Detail (LOD) in lighting calculations.
     *
     * This distance determines how close a light source must be before it starts affecting the object with full detail.
     * Closer distances can make lighting appear more detailed and realistic but may affect performance.
     *
     * @param distance The near distance for LOD in lighting calculations.
     */
    void setLightingLODNearDistance(double distance) { _lightingLODNearDistance = distance; }

    /**
     * @brief Sets the far distance for Level of Detail (LOD) in lighting calculations.
     *
     * This distance determines how far a light source affects the object with reduced detail. Increasing this distance
     * can extend the range of lighting effects but may impact performance due to more complex calculations.
     *
     * @param distance The far distance for LOD in lighting calculations.
     */
    void setLightingLODFarDistance(double distance) { _lightingLODFarDistance = distance; }

    /**
     * @brief Retrieves the title of the screen.
     *
     * This method returns the current title of the window. The title is typically displayed in the window's title bar.
     *
     * @return std::string The current title of the window.
     */
    [[nodiscard]] std::string title() const { return _title; };

    /**
     * @brief Checks if the window is currently open.
     *
     * This method returns a boolean indicating whether the window is open and visible to the user.
     *
     * @return bool True if the window is open, false otherwise.
     */
    [[nodiscard]] bool isOpen() const;

    /**
     * @brief Retrieves the width of the screen.
     *
     * This method returns the width of the screen in pixels. This width is set when the window is created and cannot be changed afterwards.
     *
     * @return uint16_t The width of the screen in pixels.
     */
    [[nodiscard]] uint16_t width() const { return _width; }

    /**
     * @brief Retrieves the height of the screen.
     *
     * This method returns the height of the screen in pixels. This height is set when the window is created and cannot be changed afterwards.
     *
     * @return uint16_t The height of the screen in pixels.
     */
    [[nodiscard]] uint16_t height() const { return _height; }

    /**
     * @brief Closes the SDL window and cleans up resources.
     *
     * This method closes the SDL window associated with the Screen object and releases any resources allocated for the window and rendering.
     * It should be called when the application is shutting down or when the window needs to be closed programmatically.
     */
    void close();

    /**
     * @brief Starts the capturing the screen.
     *
     * This method prepares starts to capture the screen to make a video clip and renders it while it is recorded.
     */
    void startRender();

    /**
     * @brief Stops the rendering process and save the final video clip.
     *
     * This method finalizes the rendering process for the current video clip and saved the rendered video in the file in your file system.
     */
    void stopRender();

    /**
     * @brief Sets the frames per second (FPS) for video rendering.
     *
     * This method sets the target FPS for video rendering. It is used to control the playback speed and performance of rendered videos.
     *
     * @param fps The target frames per second.
     */
    void setClipFps(int fps) { _clipFps = fps; }

    /**
     * @brief Sets the Constant Rate Factor (CRF) for video quality.
     *
     * This method sets the CRF for video rendering, which controls the quality and compression level of the output video. Lower values result in better quality and larger file sizes.
     *
     * @param crf The Constant Rate Factor value.
     */
    void setClipCrf(int crf) { _clipCrf = crf; }

    /**
     * @brief Captures and returns a screenshot of the current screen.
     *
     * This method captures the current state of the screen and returns it as an Image object. It can be used for taking screenshots or capturing frames for video.
     *
     * @return Image An Image object containing the screenshot.
     */
    Image makeScreenShot();

    /**
     * @brief Destructor for the Screen class.
     *
     * This destructor cleans up resources allocated by the Screen object, including closing the SDL window and releasing rendering resources.
     */
    ~Screen();
};


#endif //IO_SCREEN_H
