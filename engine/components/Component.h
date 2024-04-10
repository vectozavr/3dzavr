#ifndef COMPONENTS_COMPONENT_H
#define COMPONENTS_COMPONENT_H

#include <objects/Object.h>

class Component {
private:
    Object* _object = nullptr; // Reference to the parent Object that owns this Component

public:
    Component() = default;
    Component(const Component& component) = default;

    virtual void start() {};
    virtual void update(double deltaTime) {};
    virtual void fixedUpdate(double deltaTime) {};

    template<typename T>
    [[nodiscard]] std::shared_ptr<T> getComponent() const {
        if (_object) {
            return _object->getComponent<T>();
        }
        return nullptr;
    }

    template<typename T>
    [[nodiscard]] bool hasComponent() const {
        if (_object) {
            return _object->hasComponent<T>();
        }
        return false;
    }

    template<typename T, typename... Args>
    std::shared_ptr<T> addComponent(Args&&... args) {
        if (_object) {
            return _object->addComponent<T>(std::forward<Args>(args)...);
        }
        return nullptr;
    }

    void assignTo(Object* object) { _object = object; }
    [[nodiscard]] Object* assignedToPtr() const { return _object; }
    [[nodiscard]] std::shared_ptr<Object> assignedToShared() const { return _object->sharedPtr(); }

    [[nodiscard]] virtual std::shared_ptr<Component> copy() const {
        return std::make_shared<Component>(*this);
    }

    virtual ~Component() = default;
};

#endif //COMPONENTS_COMPONENT_H
