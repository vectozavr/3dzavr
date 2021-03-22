//
// Created by Иван Ильин on 15.03.2021.
//

#ifndef MINECRAFT_3DZAVR_OBJECT_H
#define MINECRAFT_3DZAVR_OBJECT_H

#include <vector>
#include "utils/Point4D.h"

class Object {
protected:
    std::vector<Object*> v_attached;

    Point4D p_position;
    Point4D p_angle;
public:
    Object() = default;

    virtual void translate(const Point4D& dv) {}
    virtual void rotate(const Point4D& r) {}
    virtual void rotateRelativePoint(const Point4D& point4D, const Point4D& r) {}
    virtual void rotate(const Point4D& v, double rv) {}
    virtual void rotateRelativePoint(const Point4D& s, const Point4D& v, double r) {}

    [[nodiscard]] virtual Point4D position() const { return p_position; }

    void attach(Object* object) {
        v_attached.push_back(object);
    }
};


#endif //MINECRAFT_3DZAVR_OBJECT_H
