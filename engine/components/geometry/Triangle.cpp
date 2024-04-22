#include <components/geometry/Triangle.h>
#include <linalg/Matrix3x3.h>
#include <Consts.h>

Triangle::Triangle(const std::array<Vec4D, 3>& p, const std::array<Vec3D, 3>& uv) :
    _points{p}, _textureCoordinates(uv) {
    calculateNormal();
}

void Triangle::calculateNormal() {
    auto v1 = Vec3D(_points[1] - _points[0]);
    auto v2 = Vec3D(_points[2] - _points[0]);
    Vec3D crossProduct = v1.cross(v2);

    if (crossProduct.sqrAbs() > Consts::EPS) {
        _normal = crossProduct.normalized();
    } else {
        _normal = Vec3D(0);
    }
}

Triangle Triangle::operator*(const Matrix4x4 &matrix4X4) const {
    return Triangle{{matrix4X4 * _points[0], matrix4X4 * _points[1], matrix4X4 * _points[2]}, _textureCoordinates};
}

Triangle::IntersectionInformation Triangle::intersect(const Vec3D &from, const Vec3D &to) const {
    // Möller, T. and Trumbore, B., 2005. Fast, minimum storage ray/triangle intersection. In ACM SIGGRAPH 2005 Courses (pp. 7-es).

    Vec3D pointOfIntersection;
    double a = 0;
    double b = 0;
    double distanceToTriangle = std::numeric_limits<double>::infinity();
    bool intersected = false;

    Vec3D ab = Vec3D(_points[1] - _points[0]); // E1 = v1 - v0
    Vec3D ac = Vec3D(_points[2] - _points[0]); // E2 = v2 - v0
    Vec3D ap = from - Vec3D(_points[0]);       // T  = O - v0
    Vec3D d = (to-from).normalized();               // D

    Vec3D P = d.cross(ac);
    Vec3D Q = ap.cross(ab);
    double dot = P.dot(ab);

    if(std::abs(dot) > Consts::EPS) {
        a = P.dot(ap) / dot;
        b = Q.dot(d) / dot;

        if((a >= 0) && (b >= 0) && (a + b <= 1)) {
            intersected = true;
            distanceToTriangle = Q.dot(ac) / dot;
            pointOfIntersection = from + d*distanceToTriangle;
        }
    }

    return {pointOfIntersection, Vec3D(a, b, 1-a-b), distanceToTriangle, intersected};
}

Vec3D Triangle::abgBarycCoord(const Vec2D& point) const {
    Vec2D ab = Vec2D(_points[1]) - Vec2D(_points[0]);
    Vec2D ac = Vec2D(_points[2]) - Vec2D(_points[0]);
    Vec2D ap = point - Vec2D(_points[0]);

    bool swapped = std::abs(ac.y()) < std::abs(ab.y());
    if (swapped) {
        std::swap(ab, ac);
    }

    double betta = (ap.y() * ac.x() - ap.x() * ac.y()) /
                   (ab.y() * ac.x() - ab.x() * ac.y());
    double gamma = (ap.y() - betta * ab.y()) / ac.y();
    double alpha = 1.0 - betta - gamma;

    if (swapped) {
        std::swap(betta, gamma);
    }

    return Vec3D{alpha, betta, gamma};
}

Vec3D Triangle::abgBarycCoord(const Vec3D &point) const {
    Vec3D ab = Vec3D(_points[1]) - Vec3D(_points[0]);
    Vec3D ac = Vec3D(_points[2]) - Vec3D(_points[0]);
    Vec3D ap = point - Vec3D(_points[0]);

    double len_ab = ab.abs();
    double len_ac = ac.abs();

    // Firstly we handle cases when the triangle is degenerate

    if(len_ab < Consts::EPS && len_ac < Consts::EPS) { // Triangle - is a single point
        return Vec3D{1.0, 0, 0};
    }

    if(len_ab < Consts::EPS) { // |AB| = 0
        double projection = (ap.dot(ac) / (len_ac * len_ac));
        return Vec3D{1.0 - projection, 0.0, projection};
    }

    if(len_ac < Consts::EPS) { // |AC| = 0
        double projection = (ap.dot(ab) / (len_ab * len_ab));
        return Vec3D{1.0 - projection, projection, 0.0};
    }

    Matrix3x3 system((Vec3D(_points[0])),
                     Vec3D(_points[1]),
                     Vec3D(_points[2]));

    return system.inverse()*point;
}
