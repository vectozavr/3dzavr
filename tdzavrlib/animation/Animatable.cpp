//
// Created by Иван Ильин on 26.01.2021.
//

#include "Animatable.h"
#include <iostream>

#include "ATranslate.h"
#include "ATranslateToPoint.h"
#include "AAttractToPoint.h"
#include "ARotate.h"
#include "ARotateRelativePoint.h"
#include "ARotateUpLeftLookAt.h"
#include "AScale.h"
#include "ADecompose.h"
#include "AShowCreation.h"
#include "AWait.h"

void Animatable::a_translate(const Point4D &t, double duration, Animation::LoopOut looped, Animation::InterpolationType interpolationType) {
    animations.emplace_back(new ATranslate(t, duration, looped, interpolationType));
}

void Animatable::a_translateToPoint(const Point4D &point, double duration, Animation::LoopOut looped, Animation::InterpolationType interpolationType) {
    animations.emplace_back(new ATranslateToPoint(point, duration, looped, interpolationType));
}

void Animatable::a_attractToPoint(const Point4D &point, double value, double duration, Animation::LoopOut looped, Animation::InterpolationType interpolationType) {
    animations.emplace_back(new AAttractToPoint(point, value, duration, looped, interpolationType));
}

void Animatable::a_rotate(const Point4D &r, double duration, Animation::LoopOut looped, Animation::InterpolationType interpolationType) {
    animations.emplace_back(new ARotate(r, duration, looped, interpolationType));
}

void Animatable::a_rotateRelativePoint(const Point4D &point, const Point4D &r, double duration, Animation::LoopOut looped, Animation::InterpolationType interpolationType) {
    animations.emplace_back(new ARotateRelativePoint(point, r, duration, looped, interpolationType));
}

void Animatable::a_rotateUpLeftLookAt(const Point4D &r, double duration, Animation::LoopOut looped, Animation::InterpolationType interpolationType) {
    animations.emplace_back(new ARotateUpLeftLookAt(r, duration, looped, interpolationType));
}

void Animatable::a_scale(const Point4D &s, double duration, Animation::LoopOut looped, Animation::InterpolationType interpolationType) {
    animations.emplace_back(new AScale(s, duration, looped, interpolationType));
}

void Animatable::a_scale(double s, double duration, Animation::LoopOut looped, Animation::InterpolationType interpolationType) {
    a_scale(Point4D{s, s, s}, duration, looped, interpolationType);
}

void Animatable::a_decompose(double value, double duration, Animation::LoopOut looped, Animation::InterpolationType interpolationType) {
    animations.emplace_back(new ADecompose(value, duration, looped, interpolationType));
}

void Animatable::a_showCreation(double duration, Animation::LoopOut looped, Animation::InterpolationType interpolationType) {
    animations.emplace_back(new AShowCreation(duration, looped, interpolationType));
}

void Animatable::a_wait(double duration) {
    animations.emplace_back(new AWait(duration));
}

void Animatable::a_update() {
    if(animations.empty())
        return;
    auto it = animations.begin();
    // If it the first commend is wait we should wait until waiting time is over

    if(it.operator*()->waitFor())
    {
        if(!it.operator*()->update(*this))
            animations.erase(it);
        return;
    }


    // Otherwise we iterate over all animation until we meet animations.end() or wait animation
    while(!animations.empty() && (it != animations.end()) && (!it.operator*()->waitFor()))
    {
        if(!it.operator*()->update(*this))
            animations.erase(it++);
        else
            it++;
    }
}
