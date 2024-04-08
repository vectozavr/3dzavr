#ifndef PHYSICS_HITBOX_H
#define PHYSICS_HITBOX_H

#include "components/geometry/TriangleMesh.h"
#include "components/geometry/LineMesh.h"

class HitBox final {
private:
    struct Vec3DLess {
        bool operator()(const Vec3D& lhs, const Vec3D& rhs) const noexcept;
    };

    std::vector<Vec3D> _hitBox;

    void generateSimpleFromTriangleMesh(const TriangleMesh &triangleMesh);
    void generateDetailedFromTriangleMesh(const TriangleMesh &triangleMesh);
    void generateSimpleFromLineMesh(const LineMesh &lineMesh);
    void generateDetailedFromLineMesh(const LineMesh &lineMesh);

    void constructFrom2Points(const Vec3D& from, const Vec3D& to);
public:
    HitBox() = default;
    HitBox(const HitBox &hitBox) = default;

    explicit HitBox(const TriangleMesh &triangleMesh, bool useSimpleBox = true);
    explicit HitBox(const LineMesh &lineMesh, bool useSimpleBox = true);

    [[nodiscard]] size_t size() const { return _hitBox.size(); }
    [[nodiscard]] bool empty() const { return _hitBox.empty(); }

    [[nodiscard]] std::vector<Vec3D>::iterator begin() { return _hitBox.begin(); }
    [[nodiscard]] std::vector<Vec3D>::iterator end() { return _hitBox.end(); }

    ~HitBox();
};


#endif //PHYSICS_HITBOX_H
