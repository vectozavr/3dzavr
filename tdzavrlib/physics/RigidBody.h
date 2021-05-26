//
// Created by Иван Ильин on 05.02.2021.
//

#ifndef INC_3DZAVR_RIGIDBODY_H
#define INC_3DZAVR_RIGIDBODY_H

#include <vector>
#include "../utils/Point4D.h"
#include "../Triangle.h"
#include "Simplex.h"

struct CollisionPoint {
    Point4D a; // Furthest point of a into b
    Point4D b; // Furthest point of b into a
    Point4D normal; // b – a normalized
    double depth;    // Length of b – a
    bool hasCollision;
};

class RigidBody {
private:
    Point4D p_velocity;
    Point4D p_acceleration;

    Point4D p_angularVelocity;
    Point4D p_angularAcceleration;

    double _mass = 1.0;

    bool _collision = false;
    bool _isCollider = true;

    bool _inCollision = false;

    Point4D _findFurthestPoint(const Point4D& direction);
    Point4D _support(const std::shared_ptr<RigidBody>& obj, const Point4D& direction);

    static bool _nextSimplex(Simplex& points, Point4D& direction);
    static bool _line(Simplex& points, Point4D& direction);
    static bool _triangle(Simplex& points, Point4D& direction);
    static bool _tetrahedron(Simplex& points, Point4D& direction);

    static std::pair<std::vector<Point4D>, size_t> GetFaceNormals(const std::vector<Point4D>& polytope, const std::vector<size_t>&  faces);
    static void AddIfUniqueEdge(std::vector<std::pair<size_t, size_t>>& edges, const std::vector<size_t>& faces, size_t a, size_t b);

public:
    RigidBody() = default;
    virtual ~RigidBody() = default;

    std::pair<bool, Simplex> checkGJKCollision(const std::shared_ptr<RigidBody>& obj);
    CollisionPoint EPA(const Simplex& simplex, const std::shared_ptr<RigidBody>& obj);

    [[nodiscard]] bool isCollision() const { return _collision; }
    [[nodiscard]] bool inCollision() const {return _inCollision; }
    [[nodiscard]] bool isCollider() const {return _isCollider; }
    void setInCollision(bool c) { _inCollision = c; }
    void setCollision(bool c) { _collision= c; }
    void setCollider(bool c) { _isCollider = c; }

    [[nodiscard]] virtual std::vector<Triangle>& triangles() = 0;

    [[nodiscard]] virtual Point4D position() const = 0;
    virtual void translate(const Point4D& dv) = 0;
    virtual void rotate(const Point4D& r) = 0;

    void updatePhysicsState();

    void setVelocity(const Point4D& velocity);
    void setAngularVelocity(const Point4D& angularVelocity);

    void setAcceleration(const Point4D& acceleration);
    void setAngularAcceleration(const Point4D& angularAcceleration);

    [[nodiscard]] Point4D velocity() const { return p_velocity; }

    [[nodiscard]] double mass() const { return _mass; }
    void setMass(double val) { _mass = val; }
};


#endif //INC_3DZAVR_RIGIDBODY_H
