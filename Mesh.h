//
// Created by Иван Ильин on 13.01.2021.
//

#ifndef INC_3DZAVR_MESH_H
#define INC_3DZAVR_MESH_H

#include <vector>
#include "Triangle.h"

class Mesh {
private:
    std::vector<Triangle> v_tris;

    Point4D position;
public:
    Mesh() = default;
    Mesh& operator=(const Mesh& point4D) = default;
    explicit Mesh(const std::string& filename);

    Mesh& loadObj(const std::string& filename);

    // Operations with Matrix4x4
    [[nodiscard]] Mesh operator*(const Matrix4x4& matrix4X4) const;
    Mesh& operator*=(const Matrix4x4& matrix4X4);

    std::vector<Triangle>& data() { return v_tris; }

    Mesh static Cube(double size = 1.0);
    Mesh static Obj(const std::string& filename);
};


#endif //INC_3DZAVR_MESH_H
