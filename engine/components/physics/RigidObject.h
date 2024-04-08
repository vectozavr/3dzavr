#ifndef PHYSICS_RIGIDBODY_H
#define PHYSICS_RIGIDBODY_H

#include <functional>
#include <utility>
#include <vector>
#include <memory>

#include "components/geometry/TriangleMesh.h"
#include "components/geometry/LineMesh.h"
#include "components/TransformMatrix.h"
#include "Simplex.h"
#include "HitBox.h"

struct CollisionPoint final {
    const Vec3D normal;
    const double depth;
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

// TODO: implement RigidObject collision detection and resolution for SDL version
// TODO: implement simulation of Rigid Body objects: forces, acceleration, momentum (to be able to simulate boxes, balls and etc)
// TODO: implement support of restrictions for Rigid Body objects and connections between several Rigid Body.
// TODO: implement Rag-Doll support for Rigid Body objects (bones, connections, constraints, skeleton and humanoids)


class RigidObject : public TransformMatrix {
private:
    Vec3D _velocity{0, 0, 0};
    Vec3D _acceleration{0, 0, 0};

    bool _hasCollision = false;
    bool _isCollider = true;
    bool _isTrigger = false;

    HitBox _hitBox{};
    bool _useSimpleBox = true;

    bool _inCollision = false;
    Vec3D _collisionNormal{0, 0, 0};

    Vec3D _findFurthestPoint(const Vec3D &direction);
    Vec3D _support(const std::shared_ptr<RigidObject>& obj, const Vec3D &direction);
    std::function<void(const ObjectTag &, std::shared_ptr<RigidObject>)> _collisionCallBack;

    static NextSimplex _nextSimplex(const Simplex &points);
    static NextSimplex _lineCase(const Simplex &points);
    static NextSimplex _triangleCase(const Simplex &points);
    static NextSimplex _tetrahedronCase(const Simplex &points);

    static std::pair<std::vector<FaceNormal>, size_t>
    _getFaceNormals(const std::vector<Vec3D> &polytope, const std::vector<size_t> &faces);

    static std::vector<std::pair<size_t, size_t>>
    _addIfUniqueEdge(const std::vector<std::pair<size_t, size_t>> &edges, const std::vector<size_t> &faces, size_t a,
                     size_t b);

    bool initHitBox();
public:
    explicit RigidObject(bool useSimpleBox = true) : _useSimpleBox(useSimpleBox) {};

    RigidObject(const RigidObject &rigidBody) = default;

    [[nodiscard]] std::pair<bool, Simplex> checkGJKCollision(const std::shared_ptr<RigidObject>& obj);
    [[nodiscard]] CollisionPoint EPA(const Simplex &simplex, std::shared_ptr<RigidObject> obj);
    void solveCollision(const CollisionPoint &collision);

    [[nodiscard]] Vec3D collisionNormal() const { return _collisionNormal; }
    [[nodiscard]] bool hasCollision() const { return _hasCollision; }
    [[nodiscard]] bool inCollision() const { return _inCollision; }
    [[nodiscard]] bool isCollider() const { return _isCollider; }
    [[nodiscard]] bool isTrigger() const { return _isTrigger; }

    void setInCollision(bool c) { _inCollision = c; }
    void setCollision(bool c) { _hasCollision = c; }
    void setCollider(bool c) { _isCollider = c; }
    void setTrigger(bool t) { _isTrigger = t; }

    void updatePhysicsState();

    void setVelocity(const Vec3D &velocity);
    void addVelocity(const Vec3D &velocity);
    void setAcceleration(const Vec3D &acceleration);

    [[nodiscard]] size_t hitBoxSize() const { return _hitBox.size(); }

    [[nodiscard]] Vec3D velocity() const { return _velocity; }
    [[nodiscard]] Vec3D acceleration() const { return _acceleration; }

    [[nodiscard]] const std::function<void(const ObjectTag &, std::shared_ptr<RigidObject>)> &
    collisionCallBack() const { return _collisionCallBack; }

    void setCollisionCallBack(const std::function<void(const ObjectTag &tag,
                                                       std::shared_ptr<RigidObject>)> &f) { _collisionCallBack = f; };

    [[nodiscard]] std::shared_ptr<Component> copy() const override {
        return std::make_shared<RigidObject>(*this);
    }

    void update() override;
};


#endif //PHYSICS_RIGIDBODY_H
