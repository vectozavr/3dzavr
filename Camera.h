//
// Created by Иван Ильин on 14.01.2021.
//

#ifndef INC_3DZAVR_CAMERA_H
#define INC_3DZAVR_CAMERA_H

#include <vector>
#include "Screen.h"
#include "Mesh.h"

class Camera {
private:
    Point4D p_eye;

    Point4D p_left =    {1, 0, 0, 0}; // internal X
    Point4D p_up =      {0, 1, 0, 0}; // internal Y
    Point4D p_lookAt =  {0, 0, 1, 0}; // internal Z

    Matrix4x4 P; // projections matrix
    Matrix4x4 V; // camera matrix
    Matrix4x4 S; // screen space matrix

    bool ready = false;

    std::vector<std::pair<Triangle, sf::Color>> triangles;
public:
    void init(int width, int height, double fov = 90.0, double ZNear = 0.1, double ZFar = 1000.0);
    std::vector<std::pair<Triangle, sf::Color>>& project(const Mesh &mesh);
    [[nodiscard]] std::vector<std::pair<Triangle, sf::Color>> const &data() const { return triangles; }
    void record();

    std::vector<std::pair<Triangle, sf::Color>>& sorted();

    [[nodiscard]] Point4D eye() const { return p_eye; }

    [[nodiscard]] Point4D left() const { return p_left; }
    [[nodiscard]] Point4D right() const { return -p_left; }

    [[nodiscard]] Point4D up() const { return p_up; }
    [[nodiscard]] Point4D down() const { return -p_up; }

    [[nodiscard]] Point4D lookAt() const { return p_lookAt; }

    void translate(const Point4D& dv) { p_eye += dv; }
    void translate(double dx, double dy, double dz) { p_eye += Point4D(dx, dy, dz, 0); }

    void rotateX(double rx);
    void rotateY(double ry);
    void rotateZ(double rz);
    void rotate(double rx, double ry, double rz);

    void rotate(const Point4D& v, double rv);

    void rotateLeft(double rl);
    void rotateUp(double ru);
    void rotateLookAt(double rlAt);

    void keyboardControl(Screen& screen);
};


#endif //INC_3DZAVR_CAMERA_H
