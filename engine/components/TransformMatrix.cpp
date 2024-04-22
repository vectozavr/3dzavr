#include "TransformMatrix.h"

void TransformMatrix::transform(const Matrix4x4 &t) {
    _transformMatrix = t * _transformMatrix;
}

void TransformMatrix::transformRelativePoint(const Vec3D &point, const Matrix4x4 &transform) {
    // translate object in the new coordinate system (connected with point)
    _transformMatrix = Matrix4x4::Translation( -point) * _transformMatrix;
    // transform object in the new coordinate system
    _transformMatrix = transform * _transformMatrix;
    // translate object back in self connected coordinate system
    _transformMatrix = Matrix4x4::Translation(point) * _transformMatrix;
}

void TransformMatrix::translate(const Vec3D &dv) {
    transform(Matrix4x4::Translation(dv));
}

void TransformMatrix::scale(const Vec3D &s) {
    transform(Matrix4x4::Scale(s));
}

void TransformMatrix::scaleInside(const Vec3D &s) {
    // Scale relative to the internal coordinate system
    transform(_transformMatrix*Matrix4x4::Scale(s)*Matrix4x4::View(_transformMatrix));
}

void TransformMatrix::rotate(const Vec3D &r) {
    _angle = _angle + r;

    Matrix4x4 rotationMatrix = Matrix4x4::RotationX(r.x()) * Matrix4x4::RotationY(r.y()) * Matrix4x4::RotationZ(r.z());
    transform(rotationMatrix);
}

void TransformMatrix::rotate(const Vec3D &v, double rv) {
    transform(Matrix4x4::Rotation(v, rv));
}

void TransformMatrix::rotateRelativePoint(const Vec3D &s, const Vec3D &r) {
    _angle = _angle + r;

    transformRelativePoint(s, Matrix4x4::Rotation(r));
}

void TransformMatrix::rotateRelativePoint(const Vec3D &s, const Vec3D &v, double r) {
    transformRelativePoint(s, Matrix4x4::Rotation(v, r));
}

void TransformMatrix::rotateRelativeItself(const Vec3D &r) {
    rotateRelativePoint(position(), r);
}

void TransformMatrix::rotateRelativeItself(const Vec3D &v, double r) {
    rotateRelativePoint(position(), v, r);
}

void TransformMatrix::rotateLeft(double rl) {
    _angleLeftUpLookAt = Vec3D{_angleLeftUpLookAt.x() + rl,
                               _angleLeftUpLookAt.y(),
                               _angleLeftUpLookAt.z()};

    transformRelativePoint(_transformMatrix.w(), Matrix4x4::Rotation(left(), rl));
}

void TransformMatrix::rotateUp(double ru) {
    _angleLeftUpLookAt = Vec3D{_angleLeftUpLookAt.x(),
                               _angleLeftUpLookAt.y() + ru,
                               _angleLeftUpLookAt.z()};

    transformRelativePoint(_transformMatrix.w(), Matrix4x4::Rotation(up(), ru));
}

void TransformMatrix::rotateLookAt(double rlAt) {
    _angleLeftUpLookAt = Vec3D{_angleLeftUpLookAt.x(),
                               _angleLeftUpLookAt.y(),
                               _angleLeftUpLookAt.z() + rlAt};

    transformRelativePoint(_transformMatrix.w(), Matrix4x4::Rotation(lookAt(), rlAt));
}

void TransformMatrix::translateToPoint(const Vec3D &point) {
    translate(point - position());
}

void TransformMatrix::attractToPoint(const Vec3D &point, double value) {
    Vec3D v = (point - position()).normalized();
    translate(v*value);
}

void TransformMatrix::rotateToAngle(const Vec3D &v) {
    rotate(v - _angle);
}

Matrix4x4 TransformMatrix::model() const {
    return _transformMatrix;
}

Matrix4x4 TransformMatrix::fullModel() const {
    if(!assignedToPtr() || !assignedToPtr()->attachedTo()) {
        return model();
    }

    auto attachedToTransformC = assignedToPtr()->attachedTo()->getComponent<TransformMatrix>();
    if(!attachedToTransformC) {
        return model();
    }

    return attachedToTransformC->fullModel()*model();
}
