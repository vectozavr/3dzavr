//
// Created by Иван Ильин on 29.01.2021.
//

#ifndef INC_3DZAVR_ASCALE_H
#define INC_3DZAVR_ASCALE_H

#include "Animatable.h"
#include "Animation.h"

class AScale : public Animation {
private:
    Point4D value;

    std::vector<Triangle> triangles;
public:
    AScale(const Point4D &s, double duration, LoopOut looped, InterpolationType interpolationType) {
        _duration = duration;
        _looped = looped;
        _intType = interpolationType;
        _waitFor = true;

        value = s;
    }

    bool update(Animatable& obj) override {
        if(!_started)
            triangles = obj.triangles();

        std::vector<Triangle> newTriangles;
        for(auto &t : triangles) {
            newTriangles.emplace_back(t * Matrix4x4::Scale(Point4D{1, 1, 1} + (value - Point4D{1, 1, 1}) * _p));
        }
        obj.setTriangles(newTriangles);
        return updateState();
    }
};

#endif //INC_3DZAVR_ASCALE_H
