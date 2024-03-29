#ifndef GEOMETRY_MESH_H
#define GEOMETRY_MESH_H

#include <utility>
#include <vector>

#include <objects/geometry/Triangle.h>
#include <objects/geometry/Bounds.h>
#include <objects/Object.h>
#include <objects/props/Material.h>

class TriangleMesh : public Object {
private:
    std::vector<Triangle> _tris;
    std::shared_ptr<Material> _material = Consts::DEFAULT_MATERIAL;
    Bounds _bounds;

    bool _visible = true;

    TriangleMesh &operator*=(const Matrix4x4 &matrix4X4);

    void copyTriangles(const TriangleMesh& mesh, bool deepCopy);
    void calculateBounds();

public:
    explicit TriangleMesh(const ObjectTag& nameTag) : Object(nameTag) {};

    TriangleMesh &operator=(const TriangleMesh &mesh) = delete;
    TriangleMesh(const TriangleMesh &mesh, bool deepCopy = false);
    TriangleMesh(const ObjectTag& tag, const TriangleMesh &mesh, bool deepCopy = false);

    explicit TriangleMesh(const ObjectTag& tag, const std::vector<Triangle> &tries, const std::shared_ptr<Material>& material = Consts::DEFAULT_MATERIAL);

    [[nodiscard]] std::vector<Triangle> const &triangles() const { return _tris; }

    void setTriangles(std::vector<Triangle>&& t);

    [[nodiscard]] size_t size() const { return _tris.size() * 3; }

    [[nodiscard]] std::shared_ptr<Material> getMaterial() const { return _material; }
    void setMaterial(std::shared_ptr<Material> material) { _material = std::move(material); }

    [[nodiscard]] const Bounds& bounds() const { return _bounds; }

    void setVisible(bool visibility) { _visible = visibility; }

    [[nodiscard]] bool isVisible() const { return _visible; }

    [[nodiscard]] IntersectionInformation intersect(const Vec3D &from, const Vec3D &to) override;

    std::shared_ptr<Object> copy(const ObjectTag& tag) const override {
        return std::make_shared<TriangleMesh>(tag, *this);
    }

    TriangleMesh static Surface(const ObjectTag &tag, double width, double height, const std::shared_ptr<Material>& material = nullptr);
    TriangleMesh static Cube(const ObjectTag &tag, double size = 1.0);
    TriangleMesh static LineTo(const ObjectTag &tag, const Vec3D &from, const Vec3D &to, double line_width = 0.1);
    TriangleMesh static ArrowTo(const ObjectTag &tag, const Vec3D& from, const Vec3D& to, double line_width = 1);
};

#endif //GEOMETRY_MESH_H
