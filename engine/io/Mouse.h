#ifndef IO_MOUSE_H
#define IO_MOUSE_H

#include <memory>
#include <utility>

#include <io/Screen.h>
#include <linalg/Vec2D.h>

/**
 * @file Mouse.h
 * @brief Defines the Singleton Mouse class for handling mouse input.
 *
 * This class provides static methods to interact with the mouse, including button presses,
 * mouse position, displacement, and scroll information. It uses singleton pattern to ensure
 * only one instance of the mouse handler exists.
 */

class Mouse final {
private:
    std::map<uint8_t, bool> _buttons; ///< Tracks the pressed state of mouse buttons.
    std::map<uint8_t, double> _tappedButtons; ///< Tracks buttons that have been tapped, with debounce handling.
    std::vector<uint8_t> _buttonsDown; ///< List of buttons that were pressed down in the current frame.
    std::vector<uint8_t> _buttonsUp; ///< List of buttons that were released in the current frame.

    Vec2D _motion; ///< Tracks the mouse's displacement.
    Vec2D _scroll; ///< Tracks the mouse's scroll wheel movement.

    static Mouse *_instance; ///< Singleton instance of the Mouse class.
    Mouse() = default; ///< Private default constructor to prevent direct instantiation.

public:
    /**
     * Checks if a specific mouse button is currently pressed.
     * @param button The button to check.
     * @return true if the button is pressed, false otherwise.
     */
    static bool isButtonPressed(uint8_t button);

    /**
     * Checks if a specific mouse button has been tapped.
     * A tap is considered a quick press and release, with debounce handling.
     * @param button The button to check.
     * @return true if the button has been pressed and and 1/5 sec passed from previous tap, false otherwise.
     */
    static bool isButtonTapped(uint8_t button);

    /**
     * Retrieves the current mouse position.
     * @return The current mouse position as a Vec2D.
     */
    [[nodiscard]] static Vec2D getMousePosition();

    /**
     * Retrieves the mouse's displacement since the last frame.
     * @return The mouse's displacement as a Vec2D.
     */
    [[nodiscard]] static Vec2D getMouseDisplacement();

    /**
     * Retrieves the mouse's scroll wheel movement since the last frame.
     * @return The mouse's scroll movement as a Vec2D.
     */
    [[nodiscard]] static Vec2D getMouseScroll();

    /**
     * Checks if any mouse button is currently down.
     * @return true if any button is down, false otherwise.
     */
    [[nodiscard]] static bool isButtonDown();

    /**
     * Checks if any mouse button is currently up.
     * @return true if any button is up, false otherwise.
     */
    [[nodiscard]] static bool isButtonUp();

    /**
     * Retrieves a list of buttons that were pressed down in the current frame.
     * @return A vector of button IDs that are down.
     */
    [[nodiscard]] static std::vector<uint8_t> buttonsDown();

    /**
     * Retrieves a list of buttons that were released in the current frame.
     * @return A vector of button IDs that are up.
     */
    [[nodiscard]] static std::vector<uint8_t> buttonsUp();

    /**
     * Updates the mouse's state for the current frame.
     */
    void static setMouseDisplacement(const Vec2D& displacement = Vec2D(0));

    /**
     * Updates the mouse's position for the current frame.
     */
    void static setMouseScroll(const Vec2D& scroll = Vec2D(0));

    /**
     * @brief Processes an SDL mouse event.
     *
     * This static method handles SDL mouse events, such as button presses, releases, and mouse motion.
     * It updates the internal state of the Mouse class accordingly, ensuring that mouse input is accurately
     * reflected in the application.
     *
     * @param event The SDL_Event to process, expected to be mouse-related.
     */
    static void sendMouseEvent(const SDL_Event& event);

    /**
     * @brief Initializes the mouse input handling system.
     *
     * This static method sets up the necessary state and resources for mouse input handling.
     * It should be called at the start of the application, before any mouse input is processed.
     */
    static void init();

    /**
     * @brief Clears the current mouse state.
     *
     * This static method resets the mouse state, clearing any recorded button presses, releases,
     * mouse motion, and scroll information. It is typically called at the end of a frame or update cycle
     * to prepare the mouse state for the next cycle.
     */
    static void clear();

    /**
     * @brief Frees resources associated with mouse input handling.
     *
     * This static method releases any resources or memory allocated for mouse handling.
     * It should be called when the application is shutting down or when mouse input handling is no longer needed.
     */
    static void free();
};


#endif //IO_MOUSE_H
