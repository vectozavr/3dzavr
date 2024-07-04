#ifndef COMPONENTS_COMPONENT_H
#define COMPONENTS_COMPONENT_H

#include <objects/Object.h>

/**
 * @class Component
 * @brief Base class for all components in the engine.
 *
 * This class provides the basic interface and common functionalities for components that can be attached to objects within the engine.
 * Components can perform initialization in `start`, per-frame updates in `update`, and fixed interval updates in `fixedUpdate`.
 */

class Component {
private:
    Object* _object = nullptr; ///< Reference to the parent Object that owns this Component.

public:
    /**
     * @brief Default constructor.
     */
    Component() = default;

    /**
     * @brief Copy constructor.
     * @param component The Component to copy from.
     */
    Component(const Component& component) = default;

    /**
     * @brief Virtual method for initialization logic.
     */
    virtual void start() {};

    /**
     * @brief Virtual method called every frame to update the component.
     * @param deltaTime Time elapsed since the last frame.
     */
    virtual void update(double deltaTime) {};

    /**
     * @brief Virtual method called at fixed intervals to update the component.
     * @param deltaTime Time elapsed since the last fixed update.
     */
    virtual void fixedUpdate(double deltaTime) {};

    /**
     * @brief Retrieves a component of type T from the parent object.
     * @return A shared pointer to the component of type T, or nullptr if not found.
     */
    template<typename T>
    [[nodiscard]] std::shared_ptr<T> getComponent() const {
        if (_object) {
            return _object->getComponent<T>();
        }
        return nullptr;
    }

    /**
     * @brief Checks if the parent object has a component of type T.
     * @return True if a component of type T exists, false otherwise.
     */
    template<typename T>
    [[nodiscard]] bool hasComponent() const {
        if (_object) {
            return _object->hasComponent<T>();
        }
        return false;
    }

    /**
     * @brief Adds a component of type T to the parent object.
     * @param args Arguments to forward to the component's constructor.
     * @return A shared pointer to the newly added component.
     */
    template<typename T, typename... Args>
    std::shared_ptr<T> addComponent(Args&&... args) {
        if (_object) {
            return _object->addComponent<T>(std::forward<Args>(args)...);
        }
        return nullptr;
    }

    /**
     * @brief Assigns this component to an object.
     * @param object Pointer to the Object to assign this component to.
     */
    void assignTo(Object* object) { _object = object; }

    /**
     * @brief Retrieves a pointer to the parent object.
     * @return Pointer to the parent Object.
     */
    [[nodiscard]] Object* assignedToPtr() const { return _object; }

    /**
     * @brief Retrieves a shared pointer to the parent object.
     * @return Shared pointer to the parent Object.
     */
    [[nodiscard]] std::shared_ptr<Object> assignedToShared() const { return _object->sharedPtr(); }

    /**
     * @brief Creates a copy of this component.
     * @return A shared pointer to the copied component.
     */
    [[nodiscard]] virtual std::shared_ptr<Component> copy() const {
        return std::make_shared<Component>(*this);
    }

    /**
     * @brief Default virtual destructor.
     */
    virtual ~Component() = default;
};

#endif //COMPONENTS_COMPONENT_H
