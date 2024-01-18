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
#include <objects/geometry/Texture.h>

class Mesh : public Object {
private:
    std::vector<Triangle> _tris;
    std::shared_ptr<Texture> _texture = nullptr;

    Color _color = Color(255, 245, 194);
    bool _visible = true;

    Mesh &operator*=(const Matrix4x4 &matrix4X4);

public:
    explicit Mesh(ObjectNameTag nameTag) : Object(std::move(nameTag)) {};

    Mesh &operator=(const Mesh &mesh) = delete;
    Mesh(const Mesh &mesh) = default;

    explicit Mesh(ObjectNameTag nameTag, const std::vector<Triangle> &tries, std::shared_ptr<Texture> texture = nullptr);
    explicit Mesh(ObjectNameTag nameTag,
                  const std::string &mesh_file,
                  const std::string &texture_file = "",
                  const Vec3D &scale = Vec3D{1, 1, 1});

    void loadObj(const std::string &mesh_file,
                 const std::string &texture_file = "",
                 const Vec3D &scale = Vec3D{1, 1, 1});

    [[nodiscard]] std::vector<Triangle> const &triangles() const { return _tris; }

    void setTriangles(std::vector<Triangle>&& t);

    [[nodiscard]] size_t size() const { return _tris.size() * 3; }

    [[nodiscard]] std::shared_ptr<Texture> getTexture() const { return _texture; }
    void setTexture(std::shared_ptr<Texture> texture) { _texture = texture; }

    [[nodiscard]] Color getColor() const { return _color; }
    void setColor(const Color &c);
    void setOpacity(double t);

    void setVisible(bool visibility) { _visible = visibility; }

    [[nodiscard]] bool isVisible() const { return _visible; }

    [[nodiscard]] IntersectionInformation intersect(const Vec3D &from, const Vec3D &to) const override;

    ~Mesh() override;

    Mesh static Surface(ObjectNameTag tag, double width, double height, std::shared_ptr<Texture> texture = nullptr);
    Mesh static Cube(ObjectNameTag tag, double size = 1.0, Color color = Color(255,0,0));
    Mesh static LineTo(ObjectNameTag nameTag,
                       const Vec3D &from,
                       const Vec3D &to,
                       double line_width = 0.1,
                       const Color &color = Color(150, 150, 150, 100));
    Mesh static ArrowTo(ObjectNameTag nameTag,
                        const Vec3D& from,
                        const Vec3D& to,
                        double line_width = 1,
                        Color color = Color(150, 150, 150, 255));
};

#endif //ENGINE_MESH_H
