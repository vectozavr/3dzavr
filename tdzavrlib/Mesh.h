//
// Created by Иван Ильин on 13.01.2021.
//

#ifndef INC_3DZAVR_MESH_H
#define INC_3DZAVR_MESH_H

#include <vector>
#include "Triangle.h"
#include "animation/Animatable.h"
#include "physics/RigidBody.h"
#include <SFML/Graphics.hpp>
#include "Object.h"

class Mesh : public Object, public Animatable, public RigidBody {
protected:
    std::vector<Triangle> tris;

    bool _visible = true;

    sf::Color c_color = sf::Color(255, 245, 194);

    // Operations with Matrix4x4
    [[nodiscard]] Mesh operator*(const Matrix4x4& matrix4X4) const;
    Mesh& operator*=(const Matrix4x4& matrix4X4);

public:
    Mesh() = default;
    Mesh(const Mesh& mesh);

    explicit Mesh(const std::vector<Triangle>& tries);
    Mesh& operator=(const Mesh& mesh);
    explicit Mesh(const std::string& filename);

    Mesh& loadObj(const std::string& filename, Point4D scale = {1, 1, 1});

    [[nodiscard]] std::vector<Triangle>const &triangles() const { return tris; }
    [[nodiscard]] std::vector<Triangle>& triangles() override { return tris; }
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
    void rotate(const Point4D& v, double r) override;
    // Rotate mesh around XYZ by (rx, ry, rz) radians relative val 'point4D'
    void rotateRelativePoint(const Point4D& point4D, double rx, double ry, double rz);
    // Rotate mesh around XYZ by (r.x, r.y, r.z) radians relative val 'point4D'
    void rotateRelativePoint(const Point4D& point4D, const Point4D& r) override;
    // Rotate mesh around normalised vector 'v' by 'r' radians relative val 'point4D'
    void rotateRelativePoint(const Point4D& point4D, const Point4D& v, double r) override;
    void scale(double sx, double sy, double sz);
    void scale(const Point4D& s);
    void decompose(double value) override;

    [[nodiscard]] Point4D position() const override { return p_position; }
    [[nodiscard]] Point4D angle() const override { return p_angle; }

    [[nodiscard]] sf::Color color() const { return c_color; }
    void setColor(sf::Color c);

    Mesh static Cube(double size = 1.0);
    Mesh static Obj(const std::string& filename);

    void setVisible(bool visibility) { _visible = visibility; }
    [[nodiscard]] bool isVisible() const { return _visible; }
};


#endif //INC_3DZAVR_MESH_H
