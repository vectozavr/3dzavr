//
// Created by Иван Ильин on 13.01.2021.
//

#ifndef INC_3DZAVR_MESH_H
#define INC_3DZAVR_MESH_H

#include <vector>
#include "Triangle.h"
#include "Animatable.h"
#include <SFML/Graphics.hpp>

class Mesh : public Animatable {
protected:
    std::vector<Triangle> tris;

    Point4D p_position;

    sf::Color c_color;
public:
    Mesh() = default;
    Mesh(const Mesh& mesh);

    explicit Mesh(const std::vector<Triangle>& tries);
    Mesh& operator=(const Mesh& mesh);
    explicit Mesh(const std::string& filename);

    Mesh& loadObj(const std::string& filename);

    // Operations with Matrix4x4
    [[nodiscard]] Mesh operator*(const Matrix4x4& matrix4X4) const;
    Mesh& operator*=(const Matrix4x4& matrix4X4);

    [[nodiscard]] std::vector<Triangle>const &triangles() const { return tris; }
    [[nodiscard]] std::vector<Triangle> triangles() override { return tris; }
    void setTriangles(const std::vector<Triangle>& t) override { tris = t; }

    // Translate mesh
    void translate(double dx, double dy, double dz);
    void translate(const Point4D& t) override;
    void attractToPoint(const Point4D& point, double r) override;
    void translateToPoint(const Point4D& point);
    // Rotate mesh around XYZ axes
    void rotate(double rx, double ry, double rz);
    void rotate(const Point4D& r) override;
    // Rotate mesh around normalised vector 'v' by 'r' radians
    void rotate(const Point4D& v, double r);
    // Rotate mesh around XYZ by (rx, ry, rz) radians relative val 'point4D'
    void rotateRelativePoint(const Point4D& point4D, double rx, double ry, double rz);
    // Rotate mesh around XYZ by (r.x, r.y, r.z) radians relative val 'point4D'
    void rotateRelativePoint(const Point4D& point4D, const Point4D& r) override;
    // Rotate mesh around normalised vector 'v' by 'r' radians relative val 'point4D'
    void rotateRelativePoint(const Point4D& point4D, const Point4D& v, double r);
    void scale(double sx, double sy, double sz);
    void scale(const Point4D& s);
    void decompose(double value) override;
    [[nodiscard]] Point4D position() const override { return p_position; }
    [[nodiscard]] sf::Color color() const { return c_color; }
    void setColor(sf::Color c) { c_color = c; }

    Mesh static Cube(double size = 1.0);
    Mesh static Obj(const std::string& filename);
};


#endif //INC_3DZAVR_MESH_H
