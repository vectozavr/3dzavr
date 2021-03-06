//
// Created by Иван Ильин on 12.01.2021.
//

#ifndef INC_3DZAVR_POINT4D_H
#define INC_3DZAVR_POINT4D_H

//#define STORE_POINTERS

class Point4D {
public:
    double x = 0.0;
    double y = 0.0;
    double z = 0.0;
    double w = 0.0;
#ifdef STORE_POINTERS
    double* p[4] = {&x, &y, &z, &w};
#endif

    Point4D () = default;
    Point4D (const Point4D& point4D);
    Point4D (double x, double y = 0.0, double z = 0.0, double w = 0.0);
    Point4D& operator=(const Point4D& point4D);

    [[nodiscard]] double operator[] (int i) const;
    [[nodiscard]] double& operator[] (int i);

    [[nodiscard]] Point4D operator-() const;
    [[nodiscard]] Point4D operator+() const;

    // Boolean operations
    bool operator==(const Point4D& point4D) const;
    bool operator!=(const Point4D& point4D) const;

    // Operations with Point4D
    [[nodiscard]] Point4D operator+(const Point4D& point4D) const;
    [[nodiscard]] Point4D operator-(const Point4D& point4D) const;
    [[nodiscard]] Point4D operator*(const Point4D& point4D) const;
    [[nodiscard]] Point4D operator/(const Point4D& point4D) const;

    Point4D& operator+=(const Point4D& point4D);
    Point4D& operator-=(const Point4D& point4D);
    Point4D& operator*=(const Point4D& point4D);
    Point4D& operator/=(const Point4D& point4D);

    [[nodiscard]] double dot(const Point4D& point4D) const; // Returns dot product
    [[nodiscard]] Point4D cross3D(const Point4D& point4D) const; // Returns cross product

    // Operations with numbers
    [[nodiscard]] Point4D operator+(double number) const;
    [[nodiscard]] Point4D operator-(double number) const;
    [[nodiscard]] Point4D operator*(double number) const;
    [[nodiscard]] Point4D operator/(double number) const;

    Point4D& operator+=(double number);
    Point4D& operator-=(double number);
    Point4D& operator*=(double number);
    Point4D& operator/=(double number);

    // Other useful methods
    [[nodiscard]] double sqrAbs() const; // Returns squared vector length
    [[nodiscard]] double abs() const; // Returns vector length
    Point4D& normalize(); // Returns normalized vector

    static Point4D unit_x() {return Point4D{1, 0, 0, 0}; }
    static Point4D unit_y() {return Point4D{0, 1, 0, 0}; }
    static Point4D unit_z() {return Point4D{0, 0, 1, 0}; }
};

#endif //INC_3DZAVR_POINT4D_H
