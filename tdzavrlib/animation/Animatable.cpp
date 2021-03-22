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
#include "ARotateLeftUpLookAt.h"
#include "AScale.h"
#include "ADecompose.h"
#include "AShowCreation.h"
#include "AWait.h"
#include "ARotateToAngle.h"
#include "ARotateLeftUpLookAtToAngle.h"

void Animatable::a_translate(const std::string& listName, const Point4D &t, double duration, Animation::LoopOut looped, Animation::InterpolationType interpolationType) {
    animations[listName].emplace_back(new ATranslate(t, duration, looped, interpolationType));
}

void Animatable::a_translateToPoint(const std::string& listName, const Point4D &point, double duration, Animation::LoopOut looped, Animation::InterpolationType interpolationType) {
    animations[listName].emplace_back(new ATranslateToPoint(point, duration, looped, interpolationType));
}

void Animatable::a_attractToPoint(const std::string& listName, const Point4D &point, double value, double duration, Animation::LoopOut looped, Animation::InterpolationType interpolationType) {
    animations[listName].emplace_back(new AAttractToPoint(point, value, duration, looped, interpolationType));
}

void Animatable::a_rotate(const std::string& listName, const Point4D &r, double duration, Animation::LoopOut looped, Animation::InterpolationType interpolationType) {
    animations[listName].emplace_back(new ARotate(r, duration, looped, interpolationType));
}

void Animatable::a_rotateRelativePoint(const std::string& listName, const Point4D &point, const Point4D &r, double duration, Animation::LoopOut looped, Animation::InterpolationType interpolationType) {
    animations[listName].emplace_back(new ARotateRelativePoint(point, r, duration, looped, interpolationType));
}

void Animatable::a_rotateLeftUpLookAt(const std::string& listName, const Point4D &r, double duration, Animation::LoopOut looped, Animation::InterpolationType interpolationType) {
    animations[listName].emplace_back(new ARotateLeftUpLookAt(r, duration, looped, interpolationType));
}

void Animatable::a_scale(const std::string& listName, const Point4D &s, double duration, Animation::LoopOut looped, Animation::InterpolationType interpolationType) {
    animations[listName].emplace_back(new AScale(s, duration, looped, interpolationType));
}

void Animatable::a_scale(const std::string& listName, double s, double duration, Animation::LoopOut looped, Animation::InterpolationType interpolationType) {
    a_scale(listName, Point4D{s, s, s}, duration, looped, interpolationType);
}

void Animatable::a_rotateToAngle(const std::string& listName, const Point4D &r, double duration, Animation::LoopOut looped, Animation::InterpolationType interpolationType) {
    animations[listName].emplace_back(new ARotateToAngle(r, duration, looped, interpolationType));
}

void Animatable::a_rotateLeftUpLookAtToAngle(const std::string& listName, const Point4D &r, double duration, Animation::LoopOut looped, Animation::InterpolationType interpolationType) {
    animations[listName].emplace_back(new ARotateLeftUpLookAtToAngle(r, duration, looped, interpolationType));
}


void Animatable::a_decompose(const std::string& listName, double value, double duration, Animation::LoopOut looped, Animation::InterpolationType interpolationType) {
    animations[listName].emplace_back(new ADecompose(value, duration, looped, interpolationType));
}

void Animatable::a_showCreation(const std::string& listName, double duration, Animation::LoopOut looped, Animation::InterpolationType interpolationType) {
    animations[listName].emplace_back(new AShowCreation(duration, looped, interpolationType));
}

void Animatable::a_wait(const std::string& listName, double duration) {
    animations[listName].emplace_back(new AWait(duration));
}

void Animatable::a_update() {

    for (auto& [listName, animationList] : animations) {

        if (animationList.empty())
            continue;
        auto it = animationList.begin();
        // If it the front animation is 'a_wait()' we should wait until waiting time is over

        if (it.operator*()->waitFor()) {
            if (!it.operator*()->update(*this))
                animationList.erase(it);
            return;
        }


        // Otherwise we iterate over all animation until we meet animations.end() or wait animation
        while (!animationList.empty() && (it != animationList.end()) && (!it.operator*()->waitFor())) {
            if (!it.operator*()->update(*this))
                animationList.erase(it++);
            else
                it++;
        }
    }
}
