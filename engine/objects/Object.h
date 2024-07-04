#ifndef OBJECTS_OBJECT_H
#define OBJECTS_OBJECT_H

#include <map>
#include <set>
#include <string>
#include <utility>
#include <memory>
#include <chrono>

#include <components/props/Color.h>
#include <components/geometry/Triangle.h>
#include <linalg/Matrix4x4.h>
#include <linalg/Vec3D.h>
#include <Consts.h>

class Component;

/**
 * @class ObjectTag
 * @brief Represents a tag for identifying objects.
 *
 * This class encapsulates a tag system for objects, allowing for easy identification,
 * comparison, and categorization within the game engine. It is designed to be used
 * extensively across the engine for referencing objects in a lightweight and efficient manner.
 */
class ObjectTag final {
private:
    const std::string _name; ///< The name of the tag.

public:
    /**
     * @brief Constructs an ObjectTag with an optional name.
     * @param name The name of the tag, defaulting to an empty string if not provided.
     */
    explicit ObjectTag(const std::string& name = "") : _name(name) {}

    /**
     * @brief Copy constructor.
     * @param other The other ObjectTag to copy from.
     */
    ObjectTag(const ObjectTag& other) = default;

    /**
     * @brief Returns the tag's name as a string.
     * @return The name of the tag.
     */
    [[nodiscard]] std::string str() const { return _name; }

    /**
     * @brief Checks if the tag is empty.
     * @return True if the tag's name is empty, false otherwise.
     */
    [[nodiscard]] bool empty() const { return _name.empty(); }

    /**
     * @brief Equality comparison operator.
     * @param tag The ObjectTag to compare with.
     * @return True if both tags have the same name, false otherwise.
     */
    bool operator==(const ObjectTag &tag) const { return _name == tag._name; }

    /**
     * @brief Inequality comparison operator.
     * @param tag The ObjectTag to compare with.
     * @return True if the tags have different names, false otherwise.
     */
    bool operator!=(const ObjectTag &tag) const { return _name != tag._name; }

    /**
     * @brief Less than comparison operator for ordering.
     * @param tag The ObjectTag to compare with.
     * @return True if this tag's name is lexicographically less than the other tag's name.
     */
    bool operator<(const ObjectTag &tag) const { return _name < tag._name; }

    /**
     * @brief Checks if the tag's name contains a specified substring.
     * @param str The substring to search for.
     * @return True if the substring is found within the tag's name, false otherwise.
     */
    [[nodiscard]] bool contains(const std::string& str) const;
};

/**
 * @class Object
 * @brief Represents a generic object within the game engine.
 *
 * This class serves as a base for all objects within the game engine, providing
 * common functionality such as attachment to other objects, component management,
 * and identification through tags. It leverages smart pointers for memory management
 * and enables objects to be part of a composite structure, where they can have
 * other objects attached to them or be attached to other objects.
 */
class Object : public std::enable_shared_from_this<Object> {
private:
    bool checkIfAttached(Object *obj);

    const ObjectTag _tag;

    // This is the object we are attached to
    Object* _attachedTo = nullptr;

    void copyComponentsFromObject(const Object &object);

    double _lag = 0;
    double _lastUpdate = 0;
protected:
    std::map<ObjectTag, std::shared_ptr<Object>> _attached;
    std::vector<std::shared_ptr<Component>> _components;
public:
    /**
     * Constructs an Object with a specific tag.
     * @param tag The ObjectTag to assign to this object.
     */
    explicit Object(const ObjectTag& tag);

    /**
     * Copy constructor. Creates a new Object as a copy of an existing one.
     * @param object The Object to copy from.
     */
    Object(const Object &object);

    /**
     * Constructor for creating a new Object with a specific tag, copying properties from another object.
     * @param tag The ObjectTag to assign to this new object.
     * @param object The Object from which to copy properties.
     */
    Object(const ObjectTag& tag, const Object &object);

    /**
     * Creates a copy of this object with a new tag.
     * @param tag The tag for the new object copy.
     * @return A shared pointer to the copied object.
     */
    [[nodiscard]] virtual std::shared_ptr<Object> copy(const ObjectTag& tag) const {
        return std::make_shared<Object>(tag, *this);
    }

    /**
     * Attaches an object to this object.
     * @param object Shared pointer to the object to attach.
     */
    void attach(std::shared_ptr<Object> object);

    /**
     * Attaches an object to this object by reference.
     * @param object Reference to the object to attach.
     */
    void attach(const Object& object);

    /**
     * Detaches an object with the specified tag from this object.
     * @param tag The tag of the object to detach.
     */
    void unattach(const ObjectTag &tag);

    /**
     * Retrieves a shared pointer to an attached object by its tag.
     * @param tag The tag of the object to retrieve.
     * @return Shared pointer to the attached object, or nullptr if not found.
     */
    [[nodiscard]] std::shared_ptr<Object> attached(const ObjectTag &tag);

    /**
     * Gets the pointer to the object this object is attached to.
     * @return Pointer to the parent object, or nullptr if not attached.
     */
    [[nodiscard]] Object* attachedTo() { return _attachedTo; }

    /**
     * Retrieves a shared pointer to this object.
     * @return Shared pointer to this object.
     */
    [[nodiscard]] std::shared_ptr<Object> sharedPtr() { return shared_from_this(); }

    /**
     * Counts the number of objects attached to this object.
     * @return The number of attached objects.
     */
    [[nodiscard]] uint16_t numberOfAttached() const { return _attached.size(); }

    /**
     * Detaches all objects attached to this object.
     */
    void unattachAll();

    /**
     * @brief Retrieves the object's tag name.
     * @return The tag of the object.
     */
    [[nodiscard]] ObjectTag name() const { return _tag; }

    /**
     * @brief Adds a component of type T to the object.
     * @details This function template checks if a component of type T already exists. If not, it creates a new component, assigns it to this object, and initialize it.
     * @param args Arguments to forward to the component's constructor.
     * @return A shared pointer to the newly added component.
     * @throws std::invalid_argument if a component of type T already exists.
     */
    template<typename T, typename... Args>
    std::shared_ptr<T> addComponent(Args&&... args) {
        static_assert(std::is_base_of<Component, T>::value, "T must be a Component");
        if(hasComponent<T>()) {
            throw std::invalid_argument{"Object::addComponent(): The component T had been added already"};
        }
        auto component = std::make_shared<T>(std::forward<Args>(args)...);
        component->assignTo(this);
        _components.emplace_back(component);
        component->start();
        return component;
     }

    /**
     * @brief Retrieves a component of type T from the object.
     * @return A shared pointer to the component of type T, or nullptr if not found.
     */
    template<typename T>
    [[nodiscard]] std::shared_ptr<T> getComponent() const {
        for (const auto& cmp : _components) {
            auto ptr = std::dynamic_pointer_cast<T>(cmp);
            if (ptr) {
                return ptr;
            }
        }
        return nullptr;
    }

    /**
     * @brief Checks if the object has a component of type T.
     * @return True if a component of type T exists, false otherwise.
     */
    template<typename T>
    [[nodiscard]] bool hasComponent() const {
        for (const auto& cmp : _components) {
            if (std::dynamic_pointer_cast<T>(cmp)) {
                return true;
            }
        }
        return false;
    }

    /**
     * @brief Updates all components of the object.
     */
    void updateComponents();

    /**
     * @brief Provides an iterator to the beginning of the attached objects map.
     * @return An iterator to the beginning.
     */
    std::map<ObjectTag, std::shared_ptr<Object>>::iterator begin() { return _attached.begin(); }

    /**
     * @brief Provides an iterator to the end of the attached objects map.
     * @return An iterator to the end.
     */
    std::map<ObjectTag, std::shared_ptr<Object>>::iterator end() { return _attached.end(); }

    /**
     * @brief Provides a const iterator to the beginning of the attached objects map.
     * @return A const iterator to the beginning.
     */
    std::map<ObjectTag, std::shared_ptr<Object>>::const_iterator begin() const { return _attached.begin(); }

    /**
     * @brief Provides a const iterator to the end of the attached objects map.
     * @return A const iterator to the end.
     */
    std::map<ObjectTag, std::shared_ptr<Object>>::const_iterator end() const { return _attached.end(); }

    /**
     * @brief Destructor for the Object class.
     */
    virtual ~Object();
};

#endif //OBJECTS_OBJECT_H
