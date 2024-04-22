#include <stdexcept>

#include <linalg/Matrix4x4.h>
#include <objects/Object.h>
#include <components/Component.h>
#include <utils/Time.h>

Object::Object(const ObjectTag &tag) : _tag(tag) {
}

void Object::copyComponentsFromObject(const Object &object) {
    for(const auto& cmp : object._components) {
        auto copiedComponent = cmp->copy();
        copiedComponent->assignTo(this);
        _components.emplace_back(copiedComponent);
    }
}

Object::Object(const Object &object) : _tag(object._tag) {
    copyComponentsFromObject(object);
}

Object::Object(const ObjectTag &tag, const Object &object) : _tag(tag) {
    copyComponentsFromObject(object);
}

bool ObjectTag::contains(const std::string& str) const {
    if(_name.find(str) != std::string::npos) {
        return true;
    }
    return false;
}

std::shared_ptr<Object> Object::attached(const ObjectTag &tag) {
    if (_attached.count(tag) == 0) {
        return nullptr;
    }
    return _attached[tag];
}

bool Object::checkIfAttached(Object *obj) {
    for (const auto&[nameTag, attachedObject] : _attached) {
        if (attachedObject && obj->name() == attachedObject->name() || attachedObject->checkIfAttached(obj)) {
            return true;
        }
    }
    return false;
}

void Object::attach(std::shared_ptr<Object> object) {
    if(_attached.contains(object->name())) {
        throw std::invalid_argument{"Object::attach(): You cannot inserted 2 objects with the same name tag"};
    }

    if (this != object.get()) {
        if(!object->_attachedTo) {
            if (!object->checkIfAttached(this)) {
                _attached.emplace(object->name(), object);
                object->_attachedTo = this;
            } else {
                throw std::invalid_argument{"Object::attach(): You created recursive attachment"};
            }
        } else {
            // If the object was attached to some other object we have to unnatach it at first
            // and then attach to the other:
            object->_attachedTo->unattach(object->name());
            attach(object);
        }
    } else {
        throw std::invalid_argument{"Object::attach(): You cannot attach object to itself"};
    }
}

void Object::attach(const Object &object) {
    attach(std::make_shared<Object>(object));
}

void Object::unattach(const ObjectTag &tag) {
    if(_attached.contains(tag) && _attached[tag]) {
        _attached[tag]->_attachedTo = nullptr;
    }
    _attached.erase(tag);
}

void Object::unattachAll() {
    /*
     * Here we unattach all objects (we cannot use unattach because it removes the object from the array)
     * To do this we use iterator to go through all objects.
     */
    auto it = _attached.begin();
    while (it != _attached.end()) {
        if(it->second) {
            it->second->_attachedTo = nullptr;
        }
        it = _attached.erase(it);
    }
    _attached.clear();
}

Object::~Object() {
    if(_attachedTo) {
        _attachedTo->unattach(_tag);
        _attachedTo = nullptr;
    }

    unattachAll();
}

void Object::updateComponents() {
    double deltaTime = Time::time() - _lastUpdate;
    _lastUpdate = Time::time();
    _lag += deltaTime;

    for(const auto& component : _components) {
        component->update(deltaTime);
        while (_lag >= Time::fixedDeltaTime()) {
            component->fixedUpdate(Time::fixedDeltaTime());
            _lag -= Time::fixedDeltaTime();
        }
    }
    for(const auto& [name, obj] : _attached) {
        obj->updateComponents();
    }
}
