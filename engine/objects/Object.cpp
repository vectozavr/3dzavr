#include <stdexcept>

#include "linalg/Matrix4x4.h"
#include "Object.h"

bool ObjectTag::contains(const std::string& str) const {
    if(_name.find(str) != std::string::npos) {
        return true;
    }
    return false;
}

void Object::transform(const Matrix4x4 &t) {
    _transformMatrix = t * _transformMatrix;
}

void Object::transformRelativePoint(const Vec3D &point, const Matrix4x4 &transform) {
    // translate object in the new coordinate system (connected with point)
    _transformMatrix = Matrix4x4::Translation( -point) * _transformMatrix;
    // transform object in the new coordinate system
    _transformMatrix = transform * _transformMatrix;
    // translate object back in self connected coordinate system
    _transformMatrix = Matrix4x4::Translation(point) * _transformMatrix;
}

void Object::translate(const Vec3D &dv) {
    transform(Matrix4x4::Translation(dv));
}

void Object::scale(const Vec3D &s) {
    transform(Matrix4x4::Scale(s));
}

void Object::scaleInside(const Vec3D &s) {
    // Scale relative to the internal coordinate system
    transform(_transformMatrix*Matrix4x4::Scale(s)*Matrix4x4::View(_transformMatrix));
}

void Object::rotate(const Vec3D &r) {
    _angle = _angle + r;

    Matrix4x4 rotationMatrix = Matrix4x4::RotationX(r.x()) * Matrix4x4::RotationY(r.y()) * Matrix4x4::RotationZ(r.z());
    transform(rotationMatrix);
}

void Object::rotate(const Vec3D &v, double rv) {
    transform(Matrix4x4::Rotation(v, rv));
}

void Object::rotateRelativePoint(const Vec3D &s, const Vec3D &r) {
    _angle = _angle + r;

    transformRelativePoint(s, Matrix4x4::Rotation(r));
}

void Object::rotateRelativePoint(const Vec3D &s, const Vec3D &v, double r) {
    transformRelativePoint(s, Matrix4x4::Rotation(v, r));
}

void Object::rotateLeft(double rl) {
    _angleLeftUpLookAt = Vec3D{_angleLeftUpLookAt.x() + rl,
                               _angleLeftUpLookAt.y(),
                               _angleLeftUpLookAt.z()};

    transformRelativePoint(_transformMatrix.w(), Matrix4x4::Rotation(left(), rl));
}

void Object::rotateUp(double ru) {
    _angleLeftUpLookAt = Vec3D{_angleLeftUpLookAt.x(),
                               _angleLeftUpLookAt.y() + ru,
                               _angleLeftUpLookAt.z()};

    transformRelativePoint(_transformMatrix.w(), Matrix4x4::Rotation(up(), ru));
}

void Object::rotateLookAt(double rlAt) {
    _angleLeftUpLookAt = Vec3D{_angleLeftUpLookAt.x(),
                               _angleLeftUpLookAt.y(),
                               _angleLeftUpLookAt.z() + rlAt};

    transformRelativePoint(_transformMatrix.w(), Matrix4x4::Rotation(lookAt(), rlAt));
}

void Object::translateToPoint(const Vec3D &point) {
    translate(point - position());
}

void Object::attractToPoint(const Vec3D &point, double value) {
    Vec3D v = (point - position()).normalized();
    translate(v*value);
}

void Object::rotateToAngle(const Vec3D &v) {
    rotate(v - _angle);
}

std::shared_ptr<Object> Object::attached(const ObjectTag &tag) {
    if (_attachedObjects.count(tag) == 0) {
        return nullptr;
    }
    return _attachedObjects[tag].lock();
}

bool Object::checkIfAttached(Object *obj) {
    for (const auto&[nameTag, attachedObject] : _attachedObjects) {
        if (!attachedObject.expired() && obj->name() == attachedObject.lock()->name() || attachedObject.lock()->checkIfAttached(obj)) {
            return true;
        }
    }
    return false;
}

void Object::attach(std::shared_ptr<Object> object) {
    if (this != object.get()) {
        if(!object->_attachedTo) {
            if (!object->checkIfAttached(this)) {
                _attachedObjects.emplace(object->name(), object);
                object->translateToPoint(object->fullPosition() - fullPosition());
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

void Object::unattach(const ObjectTag &tag) {
    if(_attachedObjects.contains(tag) && !_attachedObjects[tag].expired()) {
        _attachedObjects[tag].lock()->_attachedTo = nullptr;
    }
    _attachedObjects.erase(tag);
}

Matrix4x4 Object::model() const {
    return _transformMatrix;
}

Matrix4x4 Object::fullModel() const {
    if(!_attachedTo) {
        return model();
    }

    return _attachedTo->fullModel()*model();
}

Object::IntersectionInformation Object::intersect(const Vec3D &from, const Vec3D &to) {
    return IntersectionInformation{Vec3D(),
                                   Vec3D(),
                                   std::numeric_limits<double>::infinity(),
                                   nullptr,
                                   false};
}

Object::~Object() {
    if(_attachedTo) {
        _attachedTo->unattach(_tag);
        _attachedTo = nullptr;
    }

    /*
     * Here we unattach all objects (we cannot use unattach because it removes the object from the array)
     * To do this we use iterator to go through all objects.
     */
    auto it = _attachedObjects.begin();
    while (it != _attachedObjects.end()) {
        if(!it->second.expired()) {
            it->second.lock()->_attachedTo = nullptr;
        }
        it = _attachedObjects.erase(it);
    }
    _attachedObjects.clear();
}
