//
// Created by Иван Ильин on 15.03.2021.
//

#ifndef ENGINE_OBJECT_H
#define ENGINE_OBJECT_H

#include <map>
#include <set>
#include <string>
#include <utility>
#include <memory>

#include <linalg/Matrix4x4.h>
#include <linalg/Vec3D.h>
#include <objects/props/Color.h>
#include <Consts.h>
#include <objects/geometry/Triangle.h>

class ObjectTag final {
private:
    const std::string _name;
public:
    explicit ObjectTag(const std::string& name = "") : _name(name) {}

    ObjectTag(const ObjectTag& other) : _name(other._name) {}

    [[nodiscard]] std::string str() const { return _name; }
    [[nodiscard]] bool empty() const { return _name.empty(); }

    bool operator==(const ObjectTag &tag) const { return _name == tag._name; }
    bool operator!=(const ObjectTag &tag) const { return _name != tag._name; }
    bool operator<(const ObjectTag &tag) const { return _name < tag._name; }

    [[nodiscard]] bool contains(const std::string& str) const;
};


class Object : public std::enable_shared_from_this<Object> {
public:
    // TODO: maybe this structure should not be there
    struct IntersectionInformation final {
        Vec3D pointOfIntersection;
        Vec3D normal;
        double distanceToObject = std::numeric_limits<double>::infinity();
        ObjectTag objectName{};
        std::shared_ptr<Object> obj = nullptr;
        bool intersected = false;
        double k = 0;
        Color color = Consts::BLACK;
        Triangle triangle{};
    };
private:
    bool checkIfAttached(Object *obj);

    const ObjectTag _tag;

    Matrix4x4 _transformMatrix = Matrix4x4::Identity();
    /*
     * Take into account that when you rotate a body,
     * you change '_angle' & '_angleLeftUpLookAt' only for this particular body,
     * not for attached objects! Therefore, during rotations
     * '_angle' & '_angleLeftUpLookAt' stays constant for all attached objects.
     */
    Vec3D _angle{0, 0, 0};
    Vec3D _angleLeftUpLookAt{0, 0, 0};

    // This is all attached objects
    std::map<ObjectTag, std::weak_ptr<Object>> _attachedObjects;

    // This is the object we are attached to
    Object* _attachedTo = nullptr;

public:
    explicit Object(const ObjectTag& tag) : _tag(tag) {};

    Object(const Object &object) :  _tag(object._tag),
                                    _transformMatrix(object._transformMatrix),
                                    _angle(object._angle),
                                    _angleLeftUpLookAt(object._angleLeftUpLookAt) {};

    Object(const ObjectTag& tag, const Object &object) :
                                    _tag(tag),
                                    _transformMatrix(object._transformMatrix),
                                    _angle(object._angle),
                                    _angleLeftUpLookAt(object._angleLeftUpLookAt) {}

    void transform(const Matrix4x4 &t);
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

    virtual std::shared_ptr<Object> copy(const ObjectTag& tag) const {
        return std::make_shared<Object>(tag, *this);
    }

    // This function depends on Object_Type: each Object should define how does the intersection work.
    [[nodiscard]] virtual IntersectionInformation intersect(const Vec3D &from, const Vec3D &to);

    [[nodiscard]] Vec3D left() const { return _transformMatrix.x().normalized(); }
    [[nodiscard]] Vec3D up() const { return _transformMatrix.y().normalized(); }
    [[nodiscard]] Vec3D lookAt() const { return _transformMatrix.z().normalized(); }
    [[nodiscard]] Vec3D position() const { return _transformMatrix.w(); }
    [[nodiscard]] Vec3D angle() const { return _angle; }
    [[nodiscard]] Vec3D angleLeftUpLookAt() const { return _angleLeftUpLookAt; }

    void attach(std::shared_ptr<Object> object);
    void unattach(const ObjectTag &tag);
    std::shared_ptr<Object> attached(const ObjectTag &tag);

    [[nodiscard]] ObjectTag name() const { return _tag; }

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

    virtual ~Object();
};

#endif //ENGINE_OBJECT_H
