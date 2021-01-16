//
// Created by Иван Ильин on 14.01.2021.
//

#include "Camera.h"
#include "Log.h"
#include "Time.h"
#include <iostream>

std::vector<std::pair<Triangle, sf::Color>> &Camera::project(const Mesh &mesh) {

    if(!ready) {
        Log::log("Camera::project(): cannot project triangles without camera initialization ( Camera::init() ) ");
        return this->triangles;
    }

    Matrix4x4 M = Matrix4x4::Translation(mesh.position());

    for(auto& t : mesh.data()) {

        double dot = t.norm().dot((mesh.position() + t[0] - p_eye).normalize());
        if(dot > 0)
            continue;

        Triangle result = t * M * V * P * S;
        /*
         * t - one triangle from mesh
         *
         * M - translation from mesh coordinate to world coordinate
         * V - transform from world coordinate to camera' coordinate
         * P - project triangles from camera' coordinate to camera 2d plane
         * S - transform 2d plane' coordinate to screen coordinate (in pixels)
         */
        result[0] /= result[0].w;
        result[1] /= result[1].w;
        result[2] /= result[2].w;

        triangles.emplace_back(result, sf::Color(255*(0.3*abs(dot) + 0.7), 245*(0.3*abs(dot) + 0.7), 194*(0.3*abs(dot) + 0.7), 255));
    }

    return this->triangles;
}

void Camera::init(int width, int height, double fov, double ZNear, double ZFar) {
    double aspect = (double)height / (double)width;
    P = Matrix4x4::Projection(fov, aspect, ZNear, ZFar);
    S = Matrix4x4::ScreenSpace(width, height);

    ready = true;
    Log::log("Camera::init(): camera successfully initialized.");
}

std::vector<std::pair<Triangle, sf::Color>> &Camera::sorted() {

    // Sort triangles from back to front
    sort(triangles.begin(), triangles.end(), [](std::pair<Triangle, sf::Color> &t1, std::pair<Triangle, sf::Color> &t2)
    {
        double z1 = (t1.first[0].z + t1.first[1].z + t1.first[2].z) / 3.0;
        double z2 = (t2.first[0].z + t2.first[1].z + t2.first[2].z) / 3.0;
        return z1 > z2;
    });

    return triangles;
}

void Camera::record() {
    triangles.clear();

    V.setZero();
    V[0][0] = p_left[0];
    V[0][1] = p_left[1];
    V[0][2] = p_left[2];
    V[0][3] = -p_eye.dot(p_left);

    V[1][0] = p_up[0];
    V[1][1] = p_up[1];
    V[1][2] = p_up[2];
    V[1][3] = -p_eye.dot(p_up);

    V[2][0] = p_lookAt[0];
    V[2][1] = p_lookAt[1];
    V[2][2] = p_lookAt[2];
    V[2][3] = -p_eye.dot(p_lookAt);

    V[3][3] = 1.0;
}

void Camera::rotateX(double rx) {
    p_left = Matrix4x4::RotationX(rx) * p_left;
    p_up = Matrix4x4::RotationX(rx) * p_up;
    p_lookAt = Matrix4x4::RotationX(rx) * p_lookAt;
}

void Camera::rotateY(double ry) {
    p_left = Matrix4x4::RotationY(ry) * p_left;
    p_up = Matrix4x4::RotationY(ry) * p_up;
    p_lookAt = Matrix4x4::RotationY(ry) * p_lookAt;
}

void Camera::rotateZ(double rz) {
    p_left = Matrix4x4::RotationZ(rz) * p_left;
    p_up = Matrix4x4::RotationZ(rz) * p_up;
    p_lookAt = Matrix4x4::RotationZ(rz) * p_lookAt;
}

void Camera::rotate(double rx, double ry, double rz) {
    rotateX(rx);
    rotateY(ry);
    rotateZ(rz);
}

void Camera::rotate(Point4D v, double rv) {
    p_left = Matrix4x4::Rotation(v, rv) * p_left;
    p_up = Matrix4x4::Rotation(v, rv) * p_up;
    p_lookAt = Matrix4x4::Rotation(v, rv) * p_lookAt;
}

void Camera::rotateLeft(double rl) {
    rotate(p_left, rl);
}

void Camera::rotateUp(double ru) {
    rotate(p_up, ru);
}

void Camera::rotateLookAt(double rlAt) {
    rotate(p_lookAt, rlAt);
}

void Camera::keyboardControl(sf::RenderWindow& window) {
    // Left and right
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
        translate(p_left*Time::deltaTime()*5.0);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        translate(-p_left*Time::deltaTime()*5.0);
    }
    // Forward and backward
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
    {
        translate(p_lookAt*Time::deltaTime()*5.0);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
    {
        translate(-p_lookAt*Time::deltaTime()*5.0);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
    {
        translate(0.0, -Time::deltaTime()*5.0, 0);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
    {
        translate(0.0, Time::deltaTime()*5.0, 0);
    }

    // Mouse movement
    int differenceX = sf::Mouse::getPosition(window).x - (int)window.getSize().x / 2;
    int differenceY = sf::Mouse::getPosition(window).y - (int)window.getSize().y / 2;
    sf::Mouse::setPosition({ (int)window.getSize().x / 2, (int)window.getSize().y / 2 }, window);
    window.setMouseCursorVisible(false);

    rotateY(-Time::deltaTime()*differenceX/10.0);
    rotateLeft(Time::deltaTime()*differenceY/10.0);
}
