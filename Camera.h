//
// Created by Иван Ильин on 14.01.2021.
//

#ifndef INC_3DZAVR_CAMERA_H
#define INC_3DZAVR_CAMERA_H

#include <vector>
#include "Screen.h"
#include "Mesh.h"
#include "Plane.h"

class Camera {
private:
    Point4D p_eye;

    Point4D p_left =    {1, 0, 0, 0}; // internal X
    Point4D p_up =      {0, 1, 0, 0}; // internal Y
    Point4D p_lookAt =  {0, 0, 1, 0}; // internal Z

    Matrix4x4 S; // screen space matrix
    Matrix4x4 P; // projections matrix
    Matrix4x4 V; // camera matrix

    double aspect;

    // To accelerate calculations we can use precalculated matrix that does not chance
    Matrix4x4 SP; // screen-space-projections matrix
    Matrix4x4 VMA; // camera-model-animation matrix

    std::vector<Triangle> triangles;
    std::vector<Plane> clipPlanes;
    Matrix4x4 Vint; // inverse camera matrix

    bool ready = false;

    std::vector<Triangle> traced;
    bool trace = false;
    bool isExternal = false;

    double Far;
    double Near;
    double fov;
    double w;
    double h;
public:

    void init(int width, int height, double fov = 90.0, double ZNear = 0.1, double ZFar = 500.0);

    std::vector<Triangle>& project(Mesh &mesh, Screen::ViewMode mode);

    [[nodiscard]] std::vector<Triangle> const &data() const { return triangles; }
    void record();

    [[nodiscard]] int buffSize() const { return triangles.size(); }
    std::vector<Triangle>& sorted();

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
    void setTrace(bool t) { trace = t; } // Performance heavy (to observe what see camera from external camera)

    std::vector<Triangle>& tracedTriangles();

    void makeExternal() {isExternal = true;}

    [[nodiscard]] Matrix4x4& view() { return V; }
    [[nodiscard]] Matrix4x4 const & viewInv() const { return Vint; }

    [[nodiscard]] double Zfar() const {return Far;}
    [[nodiscard]] double Znear() const {return Near;}
    [[nodiscard]] double Zproj() const {return 1.0/tan(M_PI*fov*0.5/180.0);}
    [[nodiscard]] double Fov() const {return fov;}
    [[nodiscard]] double width() const {return w;}
    [[nodiscard]] double height() const {return h;}
};


#endif //INC_3DZAVR_CAMERA_H
