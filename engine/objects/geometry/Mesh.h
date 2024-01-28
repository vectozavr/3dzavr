//
// Created by Иван Ильин on 13.01.2021.
//

#ifndef ENGINE_MESH_H
#define ENGINE_MESH_H

#include <utility>
#include <vector>

#include <objects/geometry/Triangle.h>
#include <objects/Object.h>
#include <io/Image.h>
#include <objects/props/Material.h>

class Mesh : public Object {
private:
    std::vector<Triangle> _tris;
    std::shared_ptr<Material> _material = nullptr;

    bool _visible = true;

    Mesh &operator*=(const Matrix4x4 &matrix4X4);

public:
    explicit Mesh(const ObjectTag& nameTag) : Object(nameTag) {};

    Mesh &operator=(const Mesh &mesh) = delete;
    Mesh(const Mesh &mesh);
    Mesh(const ObjectTag& tag, const Mesh &mesh);

    explicit Mesh(const ObjectTag& tag, const std::vector<Triangle> &tries, std::shared_ptr<Material> material = nullptr);

    [[nodiscard]] std::vector<Triangle> const &triangles() const { return _tris; }

    void setTriangles(std::vector<Triangle>&& t);

    [[nodiscard]] size_t size() const { return _tris.size() * 3; }

    [[nodiscard]] std::shared_ptr<Material> getMaterial() const { return _material; }
    void setMaterial(std::shared_ptr<Material> material) { _material = material; }

    void setVisible(bool visibility) { _visible = visibility; }

    [[nodiscard]] bool isVisible() const { return _visible; }

    [[nodiscard]] IntersectionInformation intersect(const Vec3D &from, const Vec3D &to) const override;

    ~Mesh();

    Mesh static Surface(const ObjectTag &tag, double width, double height, std::shared_ptr<Material> material = nullptr);
    Mesh static Cube(const ObjectTag &tag, double size = 1.0);
    Mesh static LineTo(const ObjectTag &tag, const Vec3D &from, const Vec3D &to, double line_width = 0.1);
    Mesh static ArrowTo(const ObjectTag &tag, const Vec3D& from, const Vec3D& to, double line_width = 1);
};

#endif //ENGINE_MESH_H
