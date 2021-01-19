//
// Created by Иван Ильин on 13.01.2021.
//

#ifndef INC_3DZAVR_MESH_H
#define INC_3DZAVR_MESH_H

#include <vector>
#include "Triangle.h"
#include <SFML/Graphics.hpp>

class Mesh {
protected:
    std::vector<Triangle> triangles;

    Point4D p_position;

    sf::Color c_color;

    bool animation = false;
    Point4D animTranslationTranform;
    Point4D animRotationTranform;
    double endAnimationPoint = 0;
    double startAnimationPoint = 0;
public:
    Mesh() = default;
    explicit Mesh(const std::vector<Triangle>& tries);
    Mesh& operator=(const Mesh& mesh) = default;
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

    void animateTo(const Point4D& translation, const Point4D& rotation, double duration);
    Matrix4x4 animationMatrix();

    Mesh static Cube(double size = 1.0);
    Mesh static Obj(const std::string& filename);
};


#endif //INC_3DZAVR_MESH_H
