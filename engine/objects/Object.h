//
// Created by Иван Ильин on 15.03.2021.
//

#ifndef ENGINE_OBJECT_H
#define ENGINE_OBJECT_H

#include <map>
#include <string>
#include <utility>
#include <memory>

#include "linalg/Matrix4x4.h"
#include "linalg/Vec3D.h"
#include "linalg/Color.h"
#include "Consts.h"
#include "objects/geometry/Triangle.h"

class ObjectNameTag final {
private:
    const std::string _name;
public:
    explicit ObjectNameTag(std::string name = "") : _name(std::move(name)) {}

    [[nodiscard]] std::string str() const { return _name; }

    bool operator==(const ObjectNameTag &tag) const { return _name == tag._name; }
    bool operator!=(const ObjectNameTag &tag) const { return _name != tag._name; }
    bool operator<(const ObjectNameTag &tag) const { return _name < tag._name; }

    [[nodiscard]] bool contains(const ObjectNameTag& nameTag) const;
};


class Object {
public:
    // TODO: maybe this structure should not be there
    struct IntersectionInformation final {
        const Vec3D pointOfIntersection;
        const Vec3D normal;
        const double distanceToObject;
        const ObjectNameTag objectName;
        const std::shared_ptr<Object> obj;
        const bool intersected;
        const double k;
        const Color color = Consts::BLACK;
        const Triangle triangle{};
    };
private:
    bool checkIfAttached(Object *obj);

    const ObjectNameTag _nameTag;

    Matrix4x4 _transformMatrix = Matrix4x4::Identity();
    /*
     * Take into account when you rotate body,
     * you change '_angle' & '_angleLeftUpLookAt' only for this particular body,
     * but not for attached objects! This way during rotation
     * '_angle' & '_angleLeftUpLookAt' stays constant for all attached objects.
     */
    Vec3D _angle{0, 0, 0};
    Vec3D _angleLeftUpLookAt{0, 0, 0};

    std::map<ObjectNameTag, std::weak_ptr<Object>> _attachedObjects;

public:
    explicit Object(ObjectNameTag nameTag) : _nameTag(std::move(nameTag)) {};

    Object(const Object &object) :  _nameTag(object._nameTag),
                                    _transformMatrix(object._transformMatrix),
                                    _angle(object._angle),
                                    _angleLeftUpLookAt(object._angleLeftUpLookAt) {};

    // TODO: implement rotations using quaternions (?)
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

    // This function depends on Object_Type: each Object should define how does the intersection work.
    [[nodiscard]] virtual IntersectionInformation intersect(const Vec3D &from, const Vec3D &to) const;

    [[nodiscard]] Vec3D left() const { return _transformMatrix.x().normalized(); }
    [[nodiscard]] Vec3D up() const { return _transformMatrix.y().normalized(); }
    [[nodiscard]] Vec3D lookAt() const { return _transformMatrix.z().normalized(); }
    [[nodiscard]] Vec3D position() const { return _transformMatrix.w(); }
    [[nodiscard]] Vec3D angle() const { return _angle; }
    [[nodiscard]] Vec3D angleLeftUpLookAt() const { return _angleLeftUpLookAt; }

    void attach(std::shared_ptr<Object> object);
    void unattach(const ObjectNameTag &tag);
    std::shared_ptr<Object> attached(const ObjectNameTag &tag);

    [[nodiscard]] ObjectNameTag name() const { return _nameTag; }

    [[nodiscard]] Matrix4x4 model() const { return _transformMatrix; }
    /*
     * invModel() is a fast method to calculate the inverse.
     * When columns of the model() matrix are perpendicular to each other
     * invModel() will return the result of fast inverse.
     * Otherwise, it will calculate the full inverse (computationally less efficient).
     */
    [[nodiscard]] Matrix4x4 invModel() const { return Matrix4x4::View(model()); }

    virtual ~Object();
};

#endif //MINECRAFT_3DZAVR_OBJECT_H
