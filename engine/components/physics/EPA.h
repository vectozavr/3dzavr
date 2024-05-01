#ifndef PHYSICS_EPA_H
#define PHYSICS_EPA_H

#include <components/physics/GJK.h>
#include <components/geometry/Plane.h>

namespace EPA {
    struct PlanePoints final {
        std::vector<Vec3D> points;
        std::vector<Vec3D> normals;
        Vec3D origin;

        [[nodiscard]] bool isPointInside(const Vec3D& point) const;
        [[nodiscard]] std::vector<Vec3D> intersectionsWithLine(const Vec3D& a, const Vec3D& b) const;

        void computeNormals();
        void computeOrigin();
    };

    struct CollisionPoint final {
        //const Vec3D point;
        PlanePoints collisionPlane;
        const Vec3D normal;
        const double depth;
        const SupportPoint edge1;
        const SupportPoint edge2;
        const SupportPoint edge3;
        const std::vector<SupportPoint> polytope;
    };

    std::pair<std::vector<GJK::FaceNormal>, size_t>
    getFaceNormals(const std::vector<SupportPoint> &polytope, const std::vector<size_t> &faces);

    std::vector<std::pair<size_t, size_t>>
    addIfUniqueEdge(const std::vector<std::pair<size_t, size_t>> &edges, const std::vector<size_t> &faces, size_t a,
                    size_t b);

    [[nodiscard]] CollisionPoint EPA(std::shared_ptr<RigidObject> obj1,
                                     std::shared_ptr<RigidObject> obj2,
                                     const Simplex &simplex);

    [[maybe_unused]] Vec3D calculateSingleCollisionPoint(const SupportPoint& edge1,
                                                                const SupportPoint& edge2,
                                                                const SupportPoint& edge3,
                                                                const Vec3D& normal);

    PlanePoints findSortedPointsFromPlane(std::shared_ptr<RigidObject> obj, const Plane& plane);

    PlanePoints findPlanePointsIntersection(const PlanePoints& points1, const PlanePoints& points2);

    PlanePoints calculateCollisionPoints(std::shared_ptr<RigidObject> obj1,
                                                       std::shared_ptr<RigidObject> obj2,
                                                       const Vec3D& normal);

    void solveCollision(std::shared_ptr<RigidObject> obj1, std::shared_ptr<RigidObject> obj2, const Vec3D& normal, double depth);
};


#endif //PHYSICS_EPA_H
