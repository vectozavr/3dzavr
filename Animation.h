//
// Created by Иван Ильин on 23.01.2021.
//

#ifndef INC_3DZAVR_ANIMATION_H
#define INC_3DZAVR_ANIMATION_H

#include "TransformAnimation.h"
#include <list>
#include "utils/Time.h"

template <typename T>
class Animation {
private:
    T& obj;
    std::list<TransformAnimation<T>> animations;
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

    void showCreation(double duration = 1);

    void wait(double duration = 1);

    void update();

    [[nodiscard]] bool isInAnim() const { return animations.size() > 0; }
};


//
// Created by Иван Ильин on 23.01.2021.
//

template <typename T>
void Animation<T>::translate(const Point4D &t, double duration) {
    animations.emplace_back(obj, TransformAnimation<T>::transition, t, duration);
}

template <typename T>
void Animation<T>::attractToPoint(const Point4D &point, double r, double duration) {
    animations.emplace_back(obj, TransformAnimation<T>::attractToPoint, point, r, duration);
}
template <typename T>
void Animation<T>::translateToPoint(const Point4D &point, double duration) {
    animations.emplace_back(obj, TransformAnimation<T>::translateToPoint, point, duration);
}

template <typename T>
void Animation<T>::rotate(const Point4D &r, double duration) {
    animations.emplace_back(obj, TransformAnimation<T>::rotation, r, duration);
}

template <typename T>
void Animation<T>::rotateRelativePoint(const Point4D &s, const Point4D &r, double duration) {
    animations.emplace_back(obj, TransformAnimation<T>::rotateRelativePoint, s, r, duration);
}
template <typename T>
void Animation<T>::rotateUpLeftLookAt(const Point4D& r, double duration) {
    animations.emplace_back(obj, TransformAnimation<T>::rotateUpLeftLookAt, r, duration);
}

template <typename T>
void Animation<T>::scale(const Point4D &s, double duration) {
    animations.emplace_back(obj, TransformAnimation<T>::scale, s, duration);
}

template <typename T>
void Animation<T>::scale(double s, double duration) {
    scale(s, s, s, duration);
}

template<typename T>
void Animation<T>::showCreation(double duration) {
    animations.emplace_back(obj, TransformAnimation<T>::showCreation, duration);
}

template <typename T>
void Animation<T>::wait(double duration) {
    animations.emplace_back(obj, TransformAnimation<T>::wait, duration);
}

template <typename T>
void Animation<T>::update() {
    if(animations.empty())
        return;
    auto it = animations.begin();

    // If it the first commend is wait we should wait until waiting time is over
    if(it->type() == TransformAnimation<T>::wait)
    {
        if(!it->update())
            animations.erase(it);
        return;
    }
    // Otherwise we iterate over all animation until we meet animations.end() or wait animation
    while(!animations.empty() && (it != animations.end()) && (it->type() != TransformAnimation<T>::wait))
    {
        if(!it->update())
            animations.erase(it++);
        else
            it++;
    }
}

#endif //INC_3DZAVR_ANIMATION_H
