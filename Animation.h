//
// Created by Иван Ильин on 23.01.2021.
//

#ifndef INC_3DZAVR_ANIMATION_H
#define INC_3DZAVR_ANIMATION_H

#include "TransformAnimation.h"
#include <list>
#include "utils/Time.h"

template <typename T, ObjectType objectType>
class Animation {
private:
    T& obj;
    std::list<TransformAnimation<T, objectType>> animations;
public:
    explicit Animation(T& o) : obj(o) {}

    void translate(const Point4D& t, double duration = 1);
    void translateToPoint(const Point4D& point, double duration = 1);
    void attractToPoint(const Point4D& point, double r, double duration = 1);
    void rotate(const Point4D& r, double duration = 1);
    void rotateRelativePoint(const Point4D& s, const Point4D& r, double duration = 1);
    void rotateUpLeftLookAt(const Point4D& r, double duration = 1);
    // TODO: implement scale
    void scale(const Point4D& s, double duration = 1);
    void scale(double s, double duration = 1);

    void wait(double duration = 1);

    void update();

    [[nodiscard]] bool isInAnim() const { return animations.size() > 0; }
};


//
// Created by Иван Ильин on 23.01.2021.
//

template <typename T, ObjectType objectType>
void Animation<T, objectType>::translate(const Point4D &t, double duration) {
    animations.emplace_back(TransformAnimation<T, objectType>::transition, t, duration);
}

template <typename T, ObjectType objectType>
void Animation<T, objectType>::attractToPoint(const Point4D &point, double r, double duration) {
    animations.emplace_back(TransformAnimation<T, objectType>::attractToPoint, point, r, duration);
}
template <typename T, ObjectType objectType>
void Animation<T, objectType>::translateToPoint(const Point4D &point, double duration) {
    animations.emplace_back(TransformAnimation<T, objectType>::transition, point - obj.position(), duration);
}

template <typename T, ObjectType objectType>
void Animation<T, objectType>::rotate(const Point4D &r, double duration) {
    animations.emplace_back(TransformAnimation<T, objectType>::rotation, r, duration);
}

template <typename T, ObjectType objectType>
void Animation<T, objectType>::rotateRelativePoint(const Point4D &s, const Point4D &r, double duration) {
    animations.emplace_back(TransformAnimation<T, objectType>::rotateRelativePoint, s, r, duration);
}
template <typename T, ObjectType objectType>
void Animation<T, objectType>::rotateUpLeftLookAt(const Point4D& r, double duration) {
    animations.emplace_back(TransformAnimation<T, objectType>::rotateUpLeftLookAt, r, duration);
}

template <typename T, ObjectType objectType>
void Animation<T, objectType>::scale(const Point4D &s, double duration) {
    animations.emplace_back(TransformAnimation<T, objectType>::scale, s, duration);
}

template <typename T, ObjectType objectType>
void Animation<T, objectType>::scale(double s, double duration) {
    scale(s, s, s, duration);
}

template <typename T, ObjectType objectType>
void Animation<T, objectType>::wait(double duration) {
    animations.emplace_back(TransformAnimation<T, objectType>::wait, duration);
}

template <typename T, ObjectType objectType>
void Animation<T, objectType>::update() {
    if(animations.empty())
        return;
    auto it = animations.begin();

    // If it the first commend is wait we should wait until waiting time is over
    if(it->type() == TransformAnimation<T, objectType>::wait)
    {
        if(!it->update(obj))
            animations.erase(it);
        return;
    }
    // Otherwise we iterate over all animation until we meet animations.end() or wait animation
    while(!animations.empty() && (it != animations.end()) && (it->type() != TransformAnimation<T, objectType>::wait))
    {
        if(!it->update(obj))
            animations.erase(it++);
        else
            it++;
    }
}

#endif //INC_3DZAVR_ANIMATION_H
