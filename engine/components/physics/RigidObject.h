#ifndef PHYSICS_RIGIDBODY_H
#define PHYSICS_RIGIDBODY_H

#include <functional>
#include <utility>
#include <vector>
#include <memory>

#include <components/geometry/TriangleMesh.h>
#include <components/geometry/LineMesh.h>
#include <components/Component.h>
#include <components/physics/Simplex.h>
#include <components/physics/HitBox.h>
#include <linalg/Matrix3x3.h>

struct CollisionPoint final {
    const Vec3D point;
    const Vec3D normal;
    const double depth;
    const SupportPoint edge1;
    const SupportPoint edge2;
    const SupportPoint edge3;
    const std::vector<SupportPoint> polytope;
};

struct FaceNormal final {
    const Vec3D normal;
    const double distance;
};

struct NextSimplex final {
    const Simplex newSimplex;
    const Vec3D newDirection;
    const bool finishSearching;
};

// TODO: implement simulation of Rigid Body objects: forces, acceleration, momentum (to be able to simulate boxes, balls and etc)
// TODO: implement support of restrictions for Rigid Body objects and connections between several Rigid Body.
// TODO: implement Rag-Doll support for Rigid Body objects (bones, connections, constraints, skeleton and humanoids)

class RigidObject : public Component {
private:
    Vec3D _velocity{0, 0, 0};
    Vec3D _acceleration{0, 0, 0};

    double _mass = 1;
    double _volume = 0;
    double _surfaceArea = 0;
    Vec3D _centerOfMass;
    Matrix3x3 _inertiaTensor = Matrix3x3::Zero();
    Matrix3x3 _invInertiaTensor;
    double _staticFriction = 0.2;
    double _dynamicFriction = 0.15;

    bool _hasCollision = false;

    HitBox _hitBox{};
    bool _useSimpleBox = true;

    bool _inCollision = false;
    Vec3D _collisionNormal{0, 0, 0};

    std::function<void(const CollisionPoint &,
            std::shared_ptr<RigidObject>,
            std::shared_ptr<RigidObject>)> _collisionCallBack;

    SupportPoint support(const std::shared_ptr<RigidObject>& obj, const Vec3D &direction);

    static NextSimplex nextSimplex(const Simplex &points);
    static NextSimplex lineCase(const Simplex &points);
    static NextSimplex triangleCase(const Simplex &points);
    static NextSimplex tetrahedronCase(const Simplex &points);

    static std::pair<std::vector<FaceNormal>, size_t>
    getFaceNormals(const std::vector<SupportPoint> &polytope, const std::vector<size_t> &faces);

    static std::vector<std::pair<size_t, size_t>>
    addIfUniqueEdge(const std::vector<std::pair<size_t, size_t>> &edges, const std::vector<size_t> &faces, size_t a,
                    size_t b);

    bool initHitBox();
    void computeCenterOfMass(const std::shared_ptr<TriangleMesh>& triangleMesh);
    void computeInertiaTensor(const std::shared_ptr<TriangleMesh>& triangleMesh);
    void computeCenterOfMass(const std::shared_ptr<LineMesh>& lineMesh);
    void computeInertiaTensor(const std::shared_ptr<LineMesh>& lineMesh);

    static Vec3D calculateCollisionPoint(const SupportPoint& edge1,
                                         const SupportPoint& edge2,
                                         const SupportPoint& edge3,
                                         const Vec3D& normal);

public:
    explicit RigidObject(bool useSimpleBox = true) : _useSimpleBox(useSimpleBox) {};

    RigidObject(const RigidObject &rigidBody) = default;

    [[nodiscard]] std::pair<bool, Simplex> checkGJKCollision(const std::shared_ptr<RigidObject>& obj);
    [[nodiscard]] CollisionPoint EPA(const Simplex &simplex, std::shared_ptr<RigidObject> obj);

    [[nodiscard]] Vec3D collisionNormal() const { return _collisionNormal; }
    [[nodiscard]] bool hasCollision() const { return _hasCollision; }
    [[nodiscard]] bool inCollision() const { return _inCollision; }

    void setCollision(bool c) { _hasCollision = c; }

    void updatePhysicsState(double deltaTime);

    void setVelocity(const Vec3D &velocity);
    void addVelocity(const Vec3D &velocity);
    void setAcceleration(const Vec3D &acceleration);

    void setStaticFriction(double staticFriction) { _staticFriction = staticFriction; }
    void setDynamicFriction(double dynamicFriction) { _dynamicFriction = dynamicFriction; }
    void setMass(double mass) { _mass = mass; }
    [[nodiscard]] double staticFriction() const { return _staticFriction; }
    [[nodiscard]] double dynamicFriction() const { return _dynamicFriction; }
    [[nodiscard]] double mass() const { return _mass; }
    [[nodiscard]] double volume() const { return _volume; }
    [[nodiscard]] double surfaceArea() const { return _surfaceArea; }
    [[nodiscard]] Vec3D centerOfMass() const { return _centerOfMass; }
    [[nodiscard]] Matrix3x3 inertiaTensor() const { return _inertiaTensor; }
    [[nodiscard]] Matrix3x3 invInertiaTensor() const { return _invInertiaTensor; }

    [[nodiscard]] size_t hitBoxSize() const { return _hitBox.size(); }

    [[nodiscard]] Vec3D velocity() const { return _velocity; }
    [[nodiscard]] Vec3D acceleration() const { return _acceleration; }

    Vec3D findFurthestPoint(const Vec3D &direction);

    [[nodiscard]] const std::function<void(const CollisionPoint &,
            std::shared_ptr<RigidObject>,
            std::shared_ptr<RigidObject>)> &
    collisionCallBack() const { return _collisionCallBack; }

    void setCollisionCallBack(const std::function<void(const CollisionPoint &,
            std::shared_ptr<RigidObject>,
            std::shared_ptr<RigidObject>)> &f) { _collisionCallBack = f; };

    [[nodiscard]] std::shared_ptr<Component> copy() const override {
        return std::make_shared<RigidObject>(*this);
    }

    void start() override;
    void fixedUpdate(double deltaTime) override;

    static void SolveCollision(const CollisionPoint &collision,
                               const std::shared_ptr<RigidObject>& obj1,
                               const std::shared_ptr<RigidObject>& obj2);
};


#endif //PHYSICS_RIGIDBODY_H
