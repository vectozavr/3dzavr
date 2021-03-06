//
// Created by Иван Ильин on 14.01.2021.
//

#include "Camera.h"
#include "utils/Log.h"
#include "utils/Time.h"
#include <iostream>
#include <execution>

std::vector<Triangle> &Camera::project(Mesh& mesh, Screen::ViewMode mode) {

    if(!ready) {
        Log::log("Camera::project(): cannot project tris without camera initialization ( Camera::init() ) ");
        return this->triangles;
    }

    if(!mesh.isVisible())
        return this->triangles;

    // Model transform matrix: translate tris in the origin of mesh.
    Matrix4x4 M = Matrix4x4::Translation(mesh.position());
    // mesh.animationMatrix() apply one step to the animation and returns how should we
    // transform our mesh to be in intermediate state of animation
    VM = V * M;

    // We don't want to waste time re-allocating memory every time
    std::vector<Triangle> clippedTriangles, tempBuffer;
    for(auto& t : mesh.triangles()) {

        double dot = t.norm().dot((mesh.position() + t[0] - p_position).normalize());
        //double dot = t.norm().dot(p_lookAt);

        if(dot > 0 && mode != Screen::ViewMode::Xray && mode != Screen::ViewMode::Transparency &&
           mode != Screen::ViewMode::Normals && !isExternal )
            continue;

        Triangle clipped[2];
        // It needs to be cleared because it's reused through iterations. Usually it doesn't free memory.
        clippedTriangles.clear();

        // In the beginning we need to to translate triangle from world coordinate to our camera system:
        // After that we apply clipping for all planes from clipPlanes
        clippedTriangles.emplace_back(t * VM);
        for(auto& plane : clipPlanes)
        {
            while(!clippedTriangles.empty())
            {
                clipped[0] = clippedTriangles.back();
                clipped[1] = clipped[0];
                clippedTriangles.pop_back();
                int additional = plane.clip(clipped[0], clipped[1]);

                for(int i = 0; i < additional; i++)
                    tempBuffer.emplace_back(clipped[i]);
            }
            clippedTriangles.swap(tempBuffer);
        }

        for(auto& clippedTriangle : clippedTriangles) {
            // When we calculate all clipped tris it is time color them.
            // We dont need to change the color of tris in case of external camera
            // dou-to the goal of external camera is to show how see the main camera.
            // That's why we keep color of tris as it is.
            if(!isExternal) {
                //sf::Color color = clippedTriangle.
                if(mode != Screen::ViewMode::Clipped)
                    clippedTriangle.color = sf::Color(clippedTriangle.color.r * (0.3 * std::abs(dot) + 0.7), clippedTriangle.color.g * (0.3 * std::abs(dot) + 0.7), clippedTriangle.color.b * (0.3 * std::abs(dot) + 0.7), (mode == Screen::ViewMode::Transparency || mode == Screen::ViewMode::Normals) ? 100 : clippedTriangle.color.a);
                // This is for clipping demonstration.
                // If you want to debug clipping just comment previous line and uncomment this block of code.
                else {
                    if (clippedTriangle.clip == Triangle::None)
                        clippedTriangle.color = sf::Color(clippedTriangle.color.r * (0.1 * std::abs(dot) + 0.9),
                                                          clippedTriangle.color.g * (0.1 * std::abs(dot) + 0.9),
                                                          clippedTriangle.color.b * (0.1 * std::abs(dot) + 0.9), clippedTriangle.color.a);
                    else if (clippedTriangle.clip == Triangle::Cropped)
                        clippedTriangle.color = sf::Color(0, 0, 194 * (0.3 * std::abs(dot) + 0.7), 255);
                    else if (clippedTriangle.clip == Triangle::Doubled)
                        clippedTriangle.color = sf::Color(255 * (0.3 * std::abs(dot) + 0.7), 0, 0, 255);
                }
            }

            double distance1 = clippedTriangle[0].abs();
            double distance2 = clippedTriangle[1].abs();
            double distance3 = clippedTriangle[2].abs();

            // Before we projected aur clipped colored triangle, we need to save it's state
            // If we want to observe them from external camera. When we call Tdzavr::setCameraMode(CameraMode::ExternalObserver);
            // we also should call Camera::setTrace(true) in the main camera.
            if(trace) {
                // Because we will not apply Screen space transform for traced tris, we need to avoid aspect factor.
                P[0][0] *= aspect;
                Triangle prj = clippedTriangle * P;
                P[0][0] /= aspect;

                prj[0] /= prj[0].w;
                prj[1] /= prj[1].w;
                prj[2] /= prj[2].w;

                prj[0].z = Zproj();
                prj[1].z = Zproj();
                prj[2].z = Zproj();


                // We can also draw lines from points to the origin of camera
                // to show how each particular val is being projected.
                if(projectionLines) {
                    Triangle line1 = {clippedTriangle[0],
                                      clippedTriangle[0],
                                      {0, 0, 0, 1}};
                    Triangle line2 = {clippedTriangle[1],
                                      clippedTriangle[1],
                                      {0, 0, 0, 1}};
                    Triangle line3 = {clippedTriangle[2],
                                      clippedTriangle[2],
                                      {0, 0, 0, 1}};
                    traced.emplace_back(line1 * Vint);
                    traced.emplace_back(line2 * Vint);
                    traced.emplace_back(line3 * Vint);
                }

                // Draw normals in external camera
                /*
                if(mode == Screen::Normals) {
                    Triangle norm = {clippedTriangle.pos(),
                                     clippedTriangle.pos(),
                                     clippedTriangle.pos() + clippedTriangle.norm()};
                    traced.emplace_back(norm * Vint);
                }*/

                traced.emplace_back(prj * Vint);                // trace projected clipped triangle
                traced.emplace_back(clippedTriangle * Vint);    // trace clipped triangle
            }
            // You can draw normals
            if((mode == Screen::Normals) && !trace && !isExternal) {
                Triangle norm = {clippedTriangle.pos(),
                                 clippedTriangle.pos(),
                                 clippedTriangle.pos() + clippedTriangle.norm()};

                norm *= SP;
                norm[0] /= norm[0].w;
                norm[1] /= norm[1].w;
                norm[2] /= norm[2].w;

                triangles.emplace_back(norm);
            }
            // Finally its time to project our clipped colored triangle from 3D -> 2D
            // and transform it's coordinate to screen space (in pixels):
            clippedTriangle *= SP;
            /*
             * Here is the quick explanation of all main applied transformation matrix
             *
             * clippedTriangle - one clipped triangle from mesh
             *
             * M - translation from mesh coordinate to world coordinate
             * A - animation matrix (to express transitional state of the mesh) (deprecated)
             * V - transform from world coordinate to camera' coordinate
             * P - project tris from camera' coordinate to camera 2d plane
             * S - transform 2d plane' coordinate to screen coordinate (in pixels)
            */
            clippedTriangle[0] /= clippedTriangle[0].w;
            clippedTriangle[1] /= clippedTriangle[1].w;
            clippedTriangle[2] /= clippedTriangle[2].w;

            // Trying to solve incorrect triangle Z buffering
            clippedTriangle[0].z = distance1;
            clippedTriangle[1].z = distance2;
            clippedTriangle[2].z = distance3;

            triangles.emplace_back(clippedTriangle);
        }
    }

    return this->triangles;
}

void Camera::init(int width, int height, double fov, double ZNear, double ZFar) {
    // We need to init camera only after creation or changing width, height, fov, ZNear or ZFar.
    // Because here we calculate matrix that does not change during the motion of _objects or camera
    w = width; h = height;
    Near = ZNear;
    Far = ZFar;
    this->fov = fov;
    aspect = (double)width / (double)height;
    P = Matrix4x4::Projection(fov, aspect, ZNear, ZFar);
    S = Matrix4x4::ScreenSpace(width, height);

    SP = S * P; // screen-space-projections matrix

    // This is planes for clipping tris.
    // Motivation: we are not interest in tris that we cannot see.
    clipPlanes.emplace_back(Plane({0, 0, 1}, {0, 0, ZNear})); // near plane
    clipPlanes.emplace_back(Plane({0, 0, -1}, {0, 0, ZFar})); // far plane

    double thetta1 = M_PI*fov*0.5/180.0;
    double thetta2 = atan(aspect*tan(thetta1));
    double cos_theta_v = cos(thetta1);
    double sin_theta_v = sin(thetta1);
    double cos_theta_h = cos(thetta2);
    double sin_theta_h = sin(thetta2);
    clipPlanes.emplace_back(Plane({-cos_theta_h, 0, sin_theta_h}, {0, 0, 0})); // left plane
    clipPlanes.emplace_back(Plane({cos_theta_h, 0, sin_theta_h}, {0, 0, 0})); // right plane
    clipPlanes.emplace_back(Plane({0, cos_theta_v, sin_theta_v}, {0, 0, 0})); // down plane
    clipPlanes.emplace_back(Plane({0, -cos_theta_v, sin_theta_v}, {0, 0, 0})); // up plane

    ready = true;
    Log::log("Camera::init(): camera successfully initialized.");
}

std::vector<Triangle> &Camera::sorted() {

    // Sort tris from back to front
    // This is some replacement for Z-buffer
    std::sort(triangles.begin(), triangles.end(), [](Triangle &t1, Triangle &t2)
    {
        std::vector<double> v_z1({t1[0].z, t1[1].z, t1[2].z});
        std::vector<double> v_z2({t2[0].z, t2[1].z, t2[2].z});

        std::sort(v_z1.begin(), v_z1.end());
        std::sort(v_z2.begin(), v_z2.end());

        //double z1 = (t1[0].z + t1[1].z + t1[2].z);
        //double z2 = (t2[0].z + t2[1].z + t2[2].z);

        double a = 1;
        double b = 1;
        double c = 1;

        double z1 = (a*v_z1[0] + b*v_z1[1] + c*v_z1[2]);
        double z2 = (a*v_z2[0] + b*v_z2[1] + c*v_z2[2]);

        return z1 > z2;
    });

    return triangles;
}

void Camera::record() {
    // Cleaning all tris and recalculation of View matrix
    // That is like preparation for new camera shot: we need to set
    // the position of camera and insert new cartridge for photo.
    triangles.clear();
    traced.clear();

    V = Matrix4x4::View(p_left, p_up, p_lookAt, p_position);
    if(trace)
        Vint = Matrix4x4::ViewInverse(p_left, p_up, p_lookAt, p_position);
}

void Camera::rotateX(double rx) {
    p_angle.x += rx;
    p_left = Matrix4x4::RotationX(rx) * p_left;
    p_up = Matrix4x4::RotationX(rx) * p_up;
    p_lookAt = Matrix4x4::RotationX(rx) * p_lookAt;
}

void Camera::rotateY(double ry) {
    p_angle.y += ry;
    p_left = Matrix4x4::RotationY(ry) * p_left;
    p_up = Matrix4x4::RotationY(ry) * p_up;
    p_lookAt = Matrix4x4::RotationY(ry) * p_lookAt;
}

void Camera::rotateZ(double rz) {
    p_angle.z += rz;
    p_left = Matrix4x4::RotationZ(rz) * p_left;
    p_up = Matrix4x4::RotationZ(rz) * p_up;
    p_lookAt = Matrix4x4::RotationZ(rz) * p_lookAt;
}

void Camera::rotate(double rx, double ry, double rz) {
    rotateX(rx);
    rotateY(ry);
    rotateZ(rz);

    if(v_attached.empty())
        return;
    for(auto attached : v_attached)
        attached->rotateRelativePoint(position(), {rx, ry, rz});

}

void Camera::rotate(const Point4D& r) {
    rotate(r.x, r.y, r.z);
}


void Camera::rotate(const Point4D& v, double rv) {
    p_left = Matrix4x4::Rotation(v, rv) * p_left;
    p_up = Matrix4x4::Rotation(v, rv) * p_up;
    p_lookAt = Matrix4x4::Rotation(v, rv) * p_lookAt;

    if(v_attached.empty())
        return;
    for(auto& attached : v_attached)
        attached->rotateRelativePoint(position(), v, rv);
}

void Camera::rotateLeft(double rl) {
    p_angleLeftUpLookAt.x += rl;
    rotate(p_left, rl);
}

void Camera::rotateUp(double ru) {
    p_angleLeftUpLookAt.y += ru;
    rotate(p_up, ru);
}

void Camera::rotateLookAt(double rlAt) {
    p_angleLeftUpLookAt.z += rlAt;
    rotate(p_lookAt, rlAt);
}
void Camera::rotateLeftUpLookAt(const Point4D &r) {
    rotateLeft(r.x);
    rotateUp(r.y);
    rotateLookAt(r.z);
}

std::vector<Triangle> &Camera::tracedTriangles() {
    return traced;
}

void Camera::rotateRelativePoint(const Point4D &s, double rx, double ry, double rz) {
    p_angle += Point4D{rx, ry, rz};

    // Translate XYZ by vector r1
    Point4D r1 = p_position - s;
    // In translated coordinate system we rotate camera and position
    Point4D r2 = Matrix4x4::Rotation(rx, ry, rz)*r1;
    rotate(rx, ry, rz);

    // After rotation we translate XYZ by vector -r2 and recalculate position
    p_position = s + r2;

    if(v_attached.empty())
        return;
    for(auto& attached : v_attached)
        attached->rotateRelativePoint(s, {rx, ry, rz});
}

void Camera::rotateRelativePoint(const Point4D &s, const Point4D &r) {
    rotateRelativePoint(s, r.x, r.y, r.z);
}

void Camera::rotateRelativePoint(const Point4D &s, const Point4D &v, double r) {
    // Translate XYZ by vector r1
    Point4D r1 = p_position - s;
    // In translated coordinate system we rotate camera and position
    Point4D r2 = Matrix4x4::Rotation(v, r)*r1;
    rotate(v, r);

    // After rotation we translate XYZ by vector -r2 and recalculate position
    p_position = s + r2;

    if(v_attached.empty())
        return;
    for(auto& attached : v_attached)
        attached->rotateRelativePoint(s, v, r);
}

void Camera::attractToPoint(const Point4D &point, double r) {
    Point4D v = (point - p_position).normalize();
    translate(v*r);
}

void Camera::translateToPoint(const Point4D &point) {
    //p_position = point;
    translate(point - p_position);
}
