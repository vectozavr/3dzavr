//
// Created by Иван Ильин on 13.01.2021.
//

#ifndef INC_3DZAVR_MESH_H
#define INC_3DZAVR_MESH_H

#include <vector>
#include "Triangle.h"
#include <SFML/Graphics.hpp>

class Mesh {
private:
    std::vector<Triangle> triangles;

    Point4D p_position;

    sf::Color c_color;
public:
    Mesh() = default;
    Mesh& operator=(const Mesh& point4D) = default;
    explicit Mesh(const std::string& filename);

    Mesh& loadObj(const std::string& filename);

    // Operations with Matrix4x4
    [[nodiscard]] Mesh operator*(const Matrix4x4& matrix4X4) const;
    Mesh& operator*=(const Matrix4x4& matrix4X4);

    [[nodiscard]] std::vector<Triangle>const &data() const { return triangles; }
    [[nodiscard]] std::vector<Triangle>& data() { return triangles; }

    Mesh& translate(double dx, double dy, double dz);
    [[nodiscard]] Point4D const & position() const { return p_position; }
    [[nodiscard]] sf::Color color() const { return c_color; }
    void setColor(sf::Color c) { c_color = c; }

    Mesh static Cube(double size = 1.0);
    Mesh static Obj(const std::string& filename);
};


#endif //INC_3DZAVR_MESH_H
