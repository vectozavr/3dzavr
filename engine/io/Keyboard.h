#ifndef IO_KEYBOARD_H
#define IO_KEYBOARD_H

#include <map>
#include <cstdint>
#include <vector>
#include <string>

#include "SDL.h"

/**
 * @file Keyboard.h
 * @brief Defines the Singleton Keyboard class for handling keyboard inputs using SDL.
 *
 * This class provides a comprehensive interface for keyboard input management in an application.
 * It utilizes SDL for key state tracking and input text handling. The class is designed as a singleton
 * to ensure a single, global point of access to keyboard state throughout the application.
 */

class Keyboard final {
private:
    std::map<SDL_Keycode, bool> _keys; ///< Tracks the pressed state of keys.
    std::map<SDL_Keycode, double> _tappedKeys; ///< Tracks keys that have been tapped.
    std::vector<SDL_Keycode> _keysDown; ///< Stores keys that are currently pressed down.
    std::vector<SDL_Keycode> _keysUp; ///< Stores keys that have been released.
    std::string _inputText; ///< Accumulates text input from the keyboard.

    static Keyboard *_instance; ///< Singleton instance of the Keyboard class.

    Keyboard() = default; ///< Private default constructor for the singleton pattern.

public:
    Keyboard(const Keyboard &) = delete; ///< Prevents copying of the singleton instance.
    Keyboard &operator=(Keyboard &) = delete; ///< Prevents assignment to the singleton instance.

    /**
     * Checks if a specific key is currently pressed.
     * @param key The SDL_Keycode representing the key to check.
     * @return True if the key is pressed, false otherwise.
     */
    [[nodiscard]] static bool isKeyPressed(SDL_Keycode key);

    /**
     * Checks if a specific key has been tapped.
     * @param key The SDL_Keycode representing the key to check.
     * @return True if the key has been pressed and and 1/5 sec passed from previous tap, false otherwise.
     */
    [[nodiscard]] static bool isKeyTapped(SDL_Keycode key);

    /**
     * Checks if any key is currently down.
     * @return True if any key is down, false otherwise.
     */
    [[nodiscard]] static bool isKeyDown();

    /**
     * Checks if any key has been released.
     * @return True if any key is up, false otherwise.
     */
    [[nodiscard]] static bool isKeyUp();

    /**
     * Retrieves a list of keys that are currently pressed down.
     * @return A vector of SDL_Keycode representing the keys that are down.
     */
    [[nodiscard]] static std::vector<SDL_Keycode> keysDown();

    /**
     * Retrieves a list of keys that have been released.
     * @return A vector of SDL_Keycode representing the keys that are up.
     */
    [[nodiscard]] static std::vector<SDL_Keycode> keysUp();

    /**
     * Retrieves the current text input from the keyboard.
     * @return A string containing the accumulated text input.
     */
    [[nodiscard]] static std::string inputText();

    /**
     * Processes an SDL keyboard event.
     * @param event The SDL_Event to process.
     */
    static void sendKeyboardEvent(const SDL_Event& event);

    /**
     * Initializes the keyboard input system.
     */
    static void init();


    /**
     * @brief Clears the state of the keyboard.
     *
     * This method resets the keyboard state, clearing any recorded key presses, releases, and text input.
     * It is typically called at the end of a frame or update cycle to prepare the keyboard state for the next cycle.
     */
    static void clear();

    /**
     * @brief Frees resources associated with the keyboard.
     *
     * This method is responsible for releasing any resources or memory allocated for keyboard handling.
     * It should be called when the application is shutting down or when the keyboard handling is no longer needed.
     */
    static void free();
};


#endif //IO_KEYBOARD_H
