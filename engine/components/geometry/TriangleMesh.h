#ifndef GEOMETRY_MESH_H
#define GEOMETRY_MESH_H

#include <utility>
#include <vector>

#include <components/geometry/Triangle.h>
#include <components/geometry/Bounds.h>
#include <components/TransformMatrix.h>
#include <components/props/Material.h>

class TriangleMesh : public Component, public std::enable_shared_from_this<TriangleMesh> {
public:
    struct IntersectionInformation final {
        Vec3D pointOfIntersection;
        Vec3D normal;
        double distanceToObject = std::numeric_limits<double>::infinity();
        std::shared_ptr<TriangleMesh> triangleMesh = nullptr;
        bool intersected = false;
        Triangle triangle{};
    };
private:
    std::vector<Triangle> _tris;
    std::shared_ptr<Material> _material = Consts::DEFAULT_MATERIAL;
    Bounds _bounds;

    bool _visible = true;

    void copyTriangles(const TriangleMesh& mesh, bool deepCopy);
    void calculateBounds();

public:
    TriangleMesh() = default;

    TriangleMesh &operator=(const TriangleMesh &mesh) = delete;
    TriangleMesh(const TriangleMesh &mesh, bool deepCopy = false);

    explicit TriangleMesh(const std::vector<Triangle> &tries, const std::shared_ptr<Material>& material = Consts::DEFAULT_MATERIAL);

    [[nodiscard]] std::vector<Triangle> const &triangles() const { return _tris; }

    TriangleMesh &operator*=(const Matrix4x4 &matrix4X4);
    void setTriangles(std::vector<Triangle>&& t);

    [[nodiscard]] size_t size() const { return _tris.size() * 3; }

    [[nodiscard]] std::shared_ptr<Material> getMaterial() const { return _material; }
    void setMaterial(std::shared_ptr<Material> material) { _material = std::move(material); }

    [[nodiscard]] const Bounds& bounds() const { return _bounds; }

    void setVisible(bool visibility) { _visible = visibility; }

    [[nodiscard]] bool isVisible() const { return _visible; }

    [[nodiscard]] IntersectionInformation intersect(const Vec3D &from, const Vec3D &to);

    TriangleMesh static Surface(double width, double height, const std::shared_ptr<Material>& material = nullptr);
    TriangleMesh static Cube(double size = 1.0);
    TriangleMesh static LineTo(const Vec3D &from, const Vec3D &to, double line_width = 0.1);
    TriangleMesh static ArrowTo(const Vec3D& from, const Vec3D& to, double line_width = 1);

    [[nodiscard]] std::shared_ptr<Component> copy() const override {
        return std::make_shared<TriangleMesh>(*this);
    }

    void start() override;
};

#endif //GEOMETRY_MESH_H
