#ifndef PHYSICS_GJK_H
#define PHYSICS_GJK_H

#include <components/physics/Simplex.h>
#include <linalg/Vec3D.h>

class RigidObject;

namespace GJK {

    struct FaceNormal final {
        const Vec3D normal;
        const double distance;
    };

    struct NextSimplex final {
        const Simplex newSimplex;
        const Vec3D newDirection;
        const bool finishSearching;
    };

    SupportPoint support(const std::shared_ptr<RigidObject>& obj1,
                         const std::shared_ptr<RigidObject>& obj2,
                         const Vec3D &direction);

    static NextSimplex nextSimplex(const Simplex &points);
    static NextSimplex lineCase(const Simplex &points);
    static NextSimplex triangleCase(const Simplex &points);
    static NextSimplex tetrahedronCase(const Simplex &points);

    [[nodiscard]] std::pair<bool, Simplex> checkGJKCollision(const std::shared_ptr<RigidObject>& obj1,
                                                             const std::shared_ptr<RigidObject>& obj2);
};


#endif //PHYSICS_GJK_H
