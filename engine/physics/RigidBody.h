//
// Created by Иван Ильин on 05.02.2021.
//

#ifndef ENGINE_RIGIDBODY_H
#define ENGINE_RIGIDBODY_H

#include <utility>
#include <vector>
#include <memory>
#include <functional>

#include "../Triangle.h"
#include "Simplex.h"
#include "../Mesh.h"
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

class RigidBody : public Mesh {
private:
    Vec3D _velocity{0, 0, 0};
    Vec3D _acceleration{0, 0, 0};

    bool _hasCollision = false;
    bool _isCollider = true;
    bool _isTrigger = false;

    HitBox _hitBox{};

    bool _inCollision = false;
    Vec3D _collisionNormal{0, 0, 0};

    Vec3D _findFurthestPoint(const Vec3D &direction);
    Vec3D _support(std::shared_ptr<RigidBody> obj, const Vec3D &direction);
    std::function<void(const ObjectNameTag &, std::shared_ptr<RigidBody>)> _collisionCallBack;

    static NextSimplex _nextSimplex(const Simplex &points);
    static NextSimplex _lineCase(const Simplex &points);
    static NextSimplex _triangleCase(const Simplex &points);
    static NextSimplex _tetrahedronCase(const Simplex &points);

    static std::pair<std::vector<FaceNormal>, size_t>
    _getFaceNormals(const std::vector<Vec3D> &polytope, const std::vector<size_t> &faces);

    static std::vector<std::pair<size_t, size_t>>
    _addIfUniqueEdge(const std::vector<std::pair<size_t, size_t>> &edges, const std::vector<size_t> &faces, size_t a,
                     size_t b);
public:
    explicit RigidBody(ObjectNameTag nameTag) : Mesh(std::move(nameTag)) {};
    RigidBody(const RigidBody &rigidBody) = default;
    explicit RigidBody(const Mesh &mesh, bool useSimpleBox = true);
    RigidBody(ObjectNameTag nameTag, const std::string &filename, const Vec3D &scale = Vec3D{1, 1, 1}, bool useSimpleBox = true);

    [[nodiscard]] std::pair<bool, Simplex> checkGJKCollision(std::shared_ptr<RigidBody> obj);
    [[nodiscard]] CollisionPoint EPA(const Simplex &simplex, std::shared_ptr<RigidBody> obj);
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

    [[nodiscard]] Vec3D velocity() const { return _velocity; }
    [[nodiscard]] Vec3D acceleration() const { return _acceleration; }

    [[nodiscard]] const std::function<void(const ObjectNameTag &, std::shared_ptr<RigidBody>)> &
    collisionCallBack() const { return _collisionCallBack; }

    void setCollisionCallBack(const std::function<void(const ObjectNameTag &tag,
                                                       std::shared_ptr<RigidBody>)> &f) { _collisionCallBack = f; }

    ~RigidBody() override = default;
};


#endif //INC_3DZAVR_RIGIDBODY_H
