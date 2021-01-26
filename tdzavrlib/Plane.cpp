//
// Created by Иван Ильин on 19.01.2021.
//

#include "Plane.h"

Plane::Plane(const Triangle& tri) {
    triangle = tri;
    n = tri.norm();
    p = tri[0];
}

Plane::Plane(const Point4D &N, const Point4D &P) {
    n = N;
    p = P;
}

Plane::Plane(const Plane &plane) {
    triangle = plane.triangle;
    n = plane.n;
    p = plane.p;
}

double Plane::distance(const Point4D &point4D) const {
    return point4D.dot(n) - p.dot(n);
}

Point4D Plane::intersection(const Point4D &start, const Point4D &end) {
    double s_dot_n = start.dot(n);
    return start + (end - start)*(s_dot_n - p.dot(n))/(s_dot_n - end.dot(n));
}

int Plane::clip(Triangle &tri, Triangle &additional_tri) {
    n.normalize();

    Point4D insidePoints[3]; int inside = 0;
    Point4D outsidePoints[3]; int outside = 0;

    double distances[3] = {distance(tri[0]), distance(tri[1]), distance(tri[2])};

    for(int i = 0; i < 3; i++) {
        if (distances[i] >= 0)
            insidePoints[inside++] = tri[i];
        else
            outsidePoints[outside++] = tri[i];
    }

    if(inside == 0) {
        tri.clip = Triangle::Skipped;
        return 0;
    }

    if(inside == 1) {
        Point4D intersect1 = intersection(insidePoints[0], outsidePoints[0]);
        Point4D intersect2 = intersection(insidePoints[0], outsidePoints[1]);
        tri[0] = insidePoints[0];
        tri[1] = intersect1;
        tri[2] = intersect2;

        tri.clip = Triangle::Cropped;

        return 1;
    }

    if(inside == 2) {
        Point4D intersect1 = intersection(insidePoints[0], outsidePoints[0]);
        Point4D intersect2 = intersection(insidePoints[1], outsidePoints[0]);

        tri[0] = insidePoints[0];
        tri[1] = intersect1;
        tri[2] = insidePoints[1];

        additional_tri[0] = intersect1;
        additional_tri[1] = intersect2;
        additional_tri[2] = insidePoints[1];

        tri.clip = Triangle::Doubled;
        additional_tri.clip = Triangle::Doubled;

        return 2;
    }

    if(inside == 3) {
        return 1;
    }

    return 0;
}
