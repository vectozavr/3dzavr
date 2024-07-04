#ifndef ANIMATION_TIMELINE_H
#define ANIMATION_TIMELINE_H

#include <memory>
#include <string>
#include <list>
#include <map>

#include <animation/Animation.h>

/**
 * @class AnimationListTag
 * @brief Represents a tag for categorizing animations.
 *
 * This class encapsulates a tag used to categorize animations within a timeline. It provides basic
 * operations to work with the tag, including comparison and checking for emptiness.
 */
class AnimationListTag final {
private:
    const std::string _name; ///< The name of the animation list tag.
public:
    /**
     * @brief Constructs an AnimationListTag with an optional name.
     *
     * @param name The name of the tag. Defaults to an empty string if not provided.
     */
    explicit AnimationListTag(const std::string& name = "") : _name(name) {}

    /**
     * @brief Returns the name of the tag as a string.
     *
     * @return std::string The name of the tag.
     */
    [[nodiscard]] std::string str() const { return _name; }

    /**
     * @brief Checks if the tag is empty.
     *
     * @return bool True if the tag name is empty, false otherwise.
     */
    [[nodiscard]] bool empty() const { return _name.empty(); }

    /**
     * @brief Equality comparison operator.
     *
     * Compares this tag with another tag for equality based on their names.
     *
     * @param tag The other AnimationListTag to compare with.
     * @return bool True if both tags have the same name, false otherwise.
     */
    bool operator==(const AnimationListTag &tag) const { return _name == tag._name; }

    /**
     * @brief Inequality comparison operator.
     *
     * Compares this tag with another tag for inequality based on their names.
     *
     * @param tag The other AnimationListTag to compare with.
     * @return bool True if the tags have different names, false otherwise.
     */
    bool operator!=(const AnimationListTag &tag) const { return _name != tag._name; }

    /**
     * @brief Less-than comparison operator.
     *
     * Determines if this tag's name is lexicographically less than another tag's name.
     *
     * @param tag The other AnimationListTag to compare with.
     * @return bool True if this tag's name is less than the other tag's name, false otherwise.
     */
    bool operator<(const AnimationListTag &tag) const { return _name < tag._name; }
};

/**
 * @class Timeline
 * @brief Manages animations and their execution over time.
 *
 * This class is responsible for managing a collection of animations, allowing for their update,
 * addition, and deletion. It uses a singleton pattern to ensure only one instance manages the animation
 * timeline throughout the application.
 */
class Timeline {
private:
    std::map<AnimationListTag, std::list<std::shared_ptr<Animation>>> _animations; ///< Maps animation tags to lists of animations.

    static Timeline *_instance; ///< Singleton instance of the Timeline.

    Timeline() = default; ///< Private default constructor to enforce singleton pattern.
public:
    Timeline(const Timeline &) = delete;
    Timeline &operator=(Timeline &) = delete;

    /**
     * @brief Updates all animations within the timeline.
     *
     * This static method is responsible for updating the state of all animations managed by the Timeline. It should be called
     * periodically, typically within the main application loop, to advance animations based on elapsed time or other criteria.
     */
    static void update();

    /**
     * @brief Deletes all animations from the timeline.
     *
     * This static method removes all animations from the timeline, effectively clearing the animation list.
     */
    static void deleteAllAnimations();

    /**
     * @brief Deletes all animations from a specific list in the timeline.
     *
     * This static method removes all animations from a specific list in the timeline, effectively clearing that list.
     *
     * @param listName The tag of the animation list to clear.
     */
    static void deleteAnimationList(const AnimationListTag &listName);

    /**
     * @brief Checks if an animation list exists in the timeline.
     *
     * This static method determines if a specific animation list exists within the timeline.
     *
     * @param listName The tag of the animation list to check.
     * @return bool True if the animation list exists, false otherwise.
     */
    [[nodiscard]] static bool isInAnimList(const AnimationListTag &listName);

    /**
     * @brief Initializes the Timeline singleton instance.
     *
     * This static method initializes the Timeline singleton instance, allowing it to be used throughout the application.
     */
    static void init();

    /**
     * @brief Frees the Timeline singleton instance.
     *
     * This static method frees the Timeline singleton instance, releasing any resources associated with it.
     */
    static void free();

    /**
     * @brief Adds an animation to the timeline.
     *
     * This static method adds an animation to the timeline, associating it with a specific list tag.
     *
     * @tparam T The type of animation to add.
     * @tparam Arguments The arguments to pass to the animation constructor.
     * @param listName The tag of the animation list to add the animation to.
     * @param args The arguments to pass to the animation constructor.
     */
    template <typename T, typename... Arguments>
    static void addAnimation(const AnimationListTag &listName, Arguments... args) {
        if (_instance == nullptr) {
            return;
        }

        _instance->_animations[listName].emplace_back(std::make_shared<T>(args...));
    }

    /**
     * @brief Adds an animation to the timeline.
     *
     * This static method adds an animation to the timeline, associating it with the default list tag.
     *
     * @tparam T The type of animation to add.
     * @tparam Arguments The arguments to pass to the animation constructor.
     * @param args The arguments to pass to the animation constructor.
     */
    template <typename T, typename... Arguments>
    static void addAnimation(Arguments... args) {
        if (_instance == nullptr) {
            return;
        }

        _instance->_animations[AnimationListTag("timeline_0")].emplace_back(std::make_shared<T>(args...));
    }
};

#endif //ANIMATION_TIMELINE_H
