//
// Created by Иван Ильин on 04.11.2021.
//

#ifndef SHOOTER_HITBOX_H
#define SHOOTER_HITBOX_H

#include "../Mesh.h"

class HitBox final {
private:
    struct Vec3DLess {
        bool operator()(const Vec3D& lhs, const Vec3D& rhs) const noexcept;
    };

    std::vector<Vec3D> _hitBox;

    void generateSimple(const Mesh &mesh);
    void generateDetailed(const Mesh &mesh);
public:
    HitBox() = default;
    HitBox(const HitBox &hitBox) = default;

    explicit HitBox(const Mesh &mesh, bool useSimpleBox = true);

    [[nodiscard]] std::vector<Vec3D>::iterator begin() { return _hitBox.begin(); }

    [[nodiscard]] std::vector<Vec3D>::iterator end() { return _hitBox.end(); }
};


#endif //SHOOTER_HITBOX_H
