//
// Created by Иван Ильин on 29.01.2021.
//

#ifndef INC_3DZAVR_ASHOWCREATION_H
#define INC_3DZAVR_ASHOWCREATION_H

#include "Animatable.h"
#include "Animation.h"

class AShowCreation : public Animation {
private:
    std::vector<Triangle> triangles;
public:
    AShowCreation(double duration, LoopOut looped, InterpolationType interpolationType) {
        _duration = duration;
        _looped = looped;
        _intType = interpolationType;
        _waitFor = true;
    }

    bool update(Animatable& obj) {
        if(!_started)
            triangles = obj.triangles();

        std::vector<Triangle> newTriangles;
        for(auto &t : triangles) {
            double d1 = (t[1] - t[0]).abs();
            double d2 = (t[2] - t[1]).abs();
            double sum = d1 + d2;
            double a = _p * sum;
            if(a < d1)
                newTriangles.emplace_back(t[0], t[0], t[0] + (t[1] - t[0])*a/d1);
            else
                newTriangles.emplace_back(t[0], t[1], t[1] + (t[2] - t[1])*(a-d1)/d2);
        }
        obj.setTriangles(newTriangles);
        return updateState();
    }
};

#endif //INC_3DZAVR_ASHOWCREATION_H
