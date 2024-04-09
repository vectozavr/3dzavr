#ifndef COMPONENTS_TRANSFORMMATRIX_H
#define COMPONENTS_TRANSFORMMATRIX_H

#include <components/Component.h>
#include <linalg/Matrix4x4.h>

class TransformMatrix : public Component {
private:
    Matrix4x4 _transformMatrix = Matrix4x4::Identity();
    /*
     * Take into account that when you rotate a body,
     * you change '_angle' & '_angleLeftUpLookAt' only for this particular body,
     * not for attached objects! Therefore, during rotations
     * '_angle' & '_angleLeftUpLookAt' stays constant for all attached objects.
     */
    Vec3D _angle{0, 0, 0};
    Vec3D _angleLeftUpLookAt{0, 0, 0};
public:
    TransformMatrix() = default;
    TransformMatrix(const TransformMatrix& transformMatrix) = default;

    void transform(const Matrix4x4 &t);
    void undoTransformations() { transform(invModel()); }
    void transformRelativePoint(const Vec3D &point, const Matrix4x4 &transform);
    void translate(const Vec3D &dv);
    void translateToPoint(const Vec3D &point);
    void attractToPoint(const Vec3D &point, double value);
    void scale(const Vec3D &s);
    void scaleInside(const Vec3D &s);
    void rotate(const Vec3D &r);
    void rotate(const Vec3D &v, double rv);
    void rotateToAngle(const Vec3D &v);
    void rotateRelativePoint(const Vec3D &s, const Vec3D &r);
    void rotateRelativePoint(const Vec3D &s, const Vec3D &v, double r);
    void rotateLeft(double rl);
    void rotateUp(double ru);
    void rotateLookAt(double rlAt);

    [[nodiscard]] Vec3D left() const { return _transformMatrix.x().normalized(); }
    [[nodiscard]] Vec3D up() const { return _transformMatrix.y().normalized(); }
    [[nodiscard]] Vec3D lookAt() const { return _transformMatrix.z().normalized(); }
    [[nodiscard]] Vec3D position() const { return _transformMatrix.w(); }
    [[nodiscard]] Vec3D fullPosition() const { return fullModel().w(); }
    [[nodiscard]] Vec3D angle() const { return _angle; }
    [[nodiscard]] Vec3D angleLeftUpLookAt() const { return _angleLeftUpLookAt; }

    // model() returns the transform matrix of this object
    [[nodiscard]] Matrix4x4 model() const;
    /*
     * fullModel() returns the chain of transform matrices:
     * _attachedTo full transform matrix * (current transform matrix)
     */
    [[nodiscard]] Matrix4x4 fullModel() const;

    /*
     * invModel() and fullInvModel() are fast methods to calculate the inverse.
     * When columns of the model() matrix are perpendicular to each other
     * invModel() will return the result of fast inverse.
     * Otherwise, it will calculate the full inverse (computationally less efficient).
     */
    [[nodiscard]] Matrix4x4 invModel() const { return Matrix4x4::View(model()); }
    [[nodiscard]] Matrix4x4 fullInvModel() const { return Matrix4x4::View(fullModel()); }

    [[nodiscard]] std::shared_ptr<Component> copy() const override {
        return std::make_shared<TransformMatrix>(*this);
    }
};


#endif //COMPONENTS_TRANSFORMMATRIX_H
