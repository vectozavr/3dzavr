//
// Created by Иван Ильин on 15.03.2021.
//

#include <stdexcept>

#include "Object.h"
#include "math/Matrix4x4.h"

bool ObjectNameTag::contains(const ObjectNameTag &nameTag) const {
    if(_name.find(nameTag.str()) != std::string::npos) {
        return true;
    }
    return false;
}

void Object::transform(const Matrix4x4 &t) {
    _transformMatrix = t * _transformMatrix;

    for (auto &[attachedName, attachedObject] : _attachedObjects) {
        if (!attachedObject.expired()) {
            attachedObject.lock()->transformRelativePoint(position(), t);
        }
    }
}

void Object::transformRelativePoint(const Vec3D &point, const Matrix4x4 &transform) {

    // translate object in new coordinate system (connected with point)
    _transformMatrix = Matrix4x4::Translation(position() - point) * _transformMatrix;
    // transform object in the new coordinate system
    _transformMatrix = transform * _transformMatrix;
    // translate object back in self connected coordinate system
    _position = _transformMatrix.w() + point;
    _transformMatrix = Matrix4x4::Translation(-_transformMatrix.w()) * _transformMatrix;

    for (auto &[attachedName, attachedObject] : _attachedObjects) {
        if (!attachedObject.expired()) {
            attachedObject.lock()->transformRelativePoint(point, transform);
        }
    }
}

void Object::translate(const Vec3D &dv) {

    _position = _position + dv;

    for (auto &[attachedName, attachedObject] : _attachedObjects) {
        if (!attachedObject.expired()) {
            attachedObject.lock()->translate(dv);
        }
    }
}

void Object::scale(const Vec3D &s) {
    transform(Matrix4x4::Scale(s));
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

    rotate(left(), rl);
}

void Object::rotateUp(double ru) {
    _angleLeftUpLookAt = Vec3D{_angleLeftUpLookAt.x(),
                               _angleLeftUpLookAt.y() + ru,
                               _angleLeftUpLookAt.z()};

    rotate(up(), ru);
}

void Object::rotateLookAt(double rlAt) {
    _angleLeftUpLookAt = Vec3D{_angleLeftUpLookAt.x(),
                               _angleLeftUpLookAt.y(),
                               _angleLeftUpLookAt.z() + rlAt};
    rotate(lookAt(), rlAt);
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

std::shared_ptr<Object> Object::attached(const ObjectNameTag &tag) {
    if (_attachedObjects.count(tag) == 0 || _attachedObjects.find(tag)->second.expired()) {
        return nullptr;
    }
    return _attachedObjects.find(tag)->second.lock();
}

bool Object::checkIfAttached(Object *obj) {
    for (const auto&[nameTag, attachedObject] : _attachedObjects) {
        if (obj == attachedObject.lock().get() || attachedObject.lock()->checkIfAttached(obj)) {
            return true;
        }
    }
    return false;
}

void Object::attach(std::shared_ptr<Object> object) {
    if (this != object.get()) {
        if (!object->checkIfAttached(this)) {
            _attachedObjects.emplace(object->name(), object);
        } else {
            throw std::invalid_argument{"Object::attach: You tried to create infinite recursive call chains"};
        }
    } else {
        throw std::invalid_argument{"Object::attach: You cannot attach object to itself"};
    }
}

void Object::unattach(const ObjectNameTag &tag) {
    _attachedObjects.erase(tag);
}

// OpenGL function
GLfloat *Object::glInvModel() const {
    auto *v = new GLfloat[4 * 4];

    Vec3D _left = _transformMatrix.x();
    Vec3D _up = _transformMatrix.y();
    Vec3D _lookAt = _transformMatrix.z();

    v[0] = -static_cast<GLfloat>(_left.x());
    v[4] = -static_cast<GLfloat>(_left.y());
    v[8] = -static_cast<GLfloat>(_left.z());
    v[12] = static_cast<GLfloat>(position().dot(_left));

    v[1] = static_cast<GLfloat>(_up.x());
    v[5] = static_cast<GLfloat>(_up.y());
    v[9] = static_cast<GLfloat>(_up.z());
    v[13] = -static_cast<GLfloat>(position().dot(_up));

    v[2] =  -static_cast<GLfloat>(_lookAt.x());
    v[6] =  -static_cast<GLfloat>(_lookAt.y());
    v[10] = -static_cast<GLfloat>(_lookAt.z());
    v[14] = static_cast<GLfloat>(position().dot(_lookAt));

    v[3] = static_cast<GLfloat>(0.0f);
    v[7] = static_cast<GLfloat>(0.0f);
    v[11] = static_cast<GLfloat>(0.0f);
    v[15] = static_cast<GLfloat>(1.0f);

    return v;
}

GLfloat *Object::glModel() const {
    auto *m = new GLfloat[4 * 4];

    Vec3D _left = _transformMatrix.x();
    Vec3D _up = _transformMatrix.y();
    Vec3D _lookAt = _transformMatrix.z();

    m[0] = static_cast<GLfloat>(_left.x());
    m[4] = static_cast<GLfloat>(_up.x());
    m[8] = static_cast<GLfloat>(_lookAt.x());
    m[12] = static_cast<GLfloat>(position().x());

    m[1] = static_cast<GLfloat>(_left.y());
    m[5] = static_cast<GLfloat>(_up.y());
    m[9] = static_cast<GLfloat>(_lookAt.y());
    m[13] = static_cast<GLfloat>(position().y());

    m[2] = static_cast<GLfloat>(_left.z());
    m[6] = static_cast<GLfloat>(_up.z());
    m[10] = static_cast<GLfloat>(_lookAt.z());
    m[14] = static_cast<GLfloat>(position().z());

    m[3] = static_cast<GLfloat>(0.0f);
    m[7] = static_cast<GLfloat>(0.0f);
    m[11] = static_cast<GLfloat>(0.0f);
    m[15] = static_cast<GLfloat>(1.0f);

    return m;
}

Object::~Object() {
    _attachedObjects.clear();
}
