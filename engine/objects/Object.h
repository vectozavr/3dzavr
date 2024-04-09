#ifndef OBJECTS_OBJECT_H
#define OBJECTS_OBJECT_H

#include <map>
#include <set>
#include <string>
#include <utility>
#include <memory>

#include <components/props/Color.h>
#include <components/geometry/Triangle.h>
#include <linalg/Matrix4x4.h>
#include <linalg/Vec3D.h>
#include <Consts.h>

class Component;

class ObjectTag final {
private:
    const std::string _name;
public:
    explicit ObjectTag(const std::string& name = "") : _name(name) {}

    ObjectTag(const ObjectTag& other) = default;

    [[nodiscard]] std::string str() const { return _name; }
    [[nodiscard]] bool empty() const { return _name.empty(); }

    bool operator==(const ObjectTag &tag) const { return _name == tag._name; }
    bool operator!=(const ObjectTag &tag) const { return _name != tag._name; }
    bool operator<(const ObjectTag &tag) const { return _name < tag._name; }

    [[nodiscard]] bool contains(const std::string& str) const;
};


class Object : public std::enable_shared_from_this<Object> {
private:
    bool checkIfAttached(Object *obj);

    const ObjectTag _tag;

    // This is the object we are attached to
    //Object* _attachedTo = nullptr;
    Object* _attachedTo = nullptr;

    void copyComponentsFromObject(const Object &object);
protected:
    std::map<ObjectTag, std::shared_ptr<Object>> _attached;
    std::vector<std::shared_ptr<Component>> _components;
public:
    explicit Object(const ObjectTag& tag);
    Object(const Object &object);
    Object(const ObjectTag& tag, const Object &object);

    [[nodiscard]] virtual std::shared_ptr<Object> copy(const ObjectTag& tag) const {
        return std::make_shared<Object>(tag, *this);
    }

    // This function depends on Object_Type: each Object should define how does the intersection work.
    //[[nodiscard]] virtual IntersectionInformation intersect(const Vec3D &from, const Vec3D &to);

    void attach(std::shared_ptr<Object> object);
    void unattach(const ObjectTag &tag);

    [[nodiscard]] std::shared_ptr<Object> attached(const ObjectTag &tag);
    [[nodiscard]] Object* attachedTo() { return _attachedTo; }
    [[nodiscard]] std::shared_ptr<Object> sharedPtr() { return shared_from_this(); }
    [[nodiscard]] uint16_t numberOfAttached() const { return _attached.size(); }

    void unattachAll();

    [[nodiscard]] ObjectTag name() const { return _tag; }

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

    template<typename T>
    [[nodiscard]] bool hasComponent() const {
        for (const auto& cmp : _components) {
            if (std::dynamic_pointer_cast<T>(cmp)) {
                return true;
            }
        }
        return false;
    }

    void updateComponents();

    std::map<ObjectTag, std::shared_ptr<Object>>::iterator begin() { return _attached.begin(); }
    std::map<ObjectTag, std::shared_ptr<Object>>::iterator end() { return _attached.end(); }
    std::map<ObjectTag, std::shared_ptr<Object>>::const_iterator begin() const { return _attached.begin(); }
    std::map<ObjectTag, std::shared_ptr<Object>>::const_iterator end() const { return _attached.end(); }

    virtual ~Object();
};

#endif //OBJECTS_OBJECT_H
