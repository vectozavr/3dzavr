//
// Created by Иван Ильин on 13.01.2021.
//

#ifndef INC_3DZAVR_MESH_H
#define INC_3DZAVR_MESH_H

#include <vector>
#include "Triangle.h"
#include "Animation.h"
#include <SFML/Graphics.hpp>

class Mesh {
protected:
    std::vector<Triangle> triangles;

    Point4D p_position;

    sf::Color c_color;
public:
    Mesh() = default;
    explicit Mesh(const std::vector<Triangle>& tries);
    Mesh& operator=(const Mesh& mesh);
    explicit Mesh(const std::string& filename);

    Mesh& loadObj(const std::string& filename);

    // Operations with Matrix4x4
    [[nodiscard]] Mesh operator*(const Matrix4x4& matrix4X4) const;
    Mesh& operator*=(const Matrix4x4& matrix4X4);

    [[nodiscard]] std::vector<Triangle>const &data() const { return triangles; }
    [[nodiscard]] std::vector<Triangle>& data() { return triangles; }

    Mesh& translate(double dx, double dy, double dz);
    Mesh& translate(const Point4D& t);
    Mesh& rotate(double rx, double ry, double rz);
    Mesh& rotate(const Point4D& r);
    Mesh& rotate(const Point4D& v, double r);
    Mesh& scale(double sx, double sy, double sz);
    Mesh& scale(const Point4D& s);
    [[nodiscard]] Point4D const & position() const { return p_position; }
    [[nodiscard]] sf::Color color() const { return c_color; }
    void setColor(sf::Color c) { c_color = c; }

    Mesh static Cube(double size = 1.0);
    Mesh static Obj(const std::string& filename);

    Animation<Mesh> animation;
};


#endif //INC_3DZAVR_MESH_H
