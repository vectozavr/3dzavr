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
    std::list<TransformAnimation<T>> animations;
public:
    void translate(const Point4D& t, double duration = 1);
    void translate(double dx, double dy, double dz, double duration = 1);
    void rotate(const Point4D& r, double duration = 1);
    void rotate(double rx, double ry, double rz, double duration = 1);
    void scale(const Point4D& s, double duration = 1);
    void scale(double sx, double sy, double sz, double duration = 1);
    void scale(double s, double duration = 1);

    void wait(double duration = 1);

    void update(T& obj);

    [[nodiscard]] bool isInAnim() const { return animations.size() > 0; }
};


//
// Created by Иван Ильин on 23.01.2021.
//

template <typename T>
void Animation<T>::translate(const Point4D &t, double duration) {
    animations.emplace_back(TransformAnimation<T>::transition, t, duration);
}
template <typename T>
void Animation<T>::translate(double dx, double dy, double dz, double duration) {
    translate({dx, dy, dz}, duration);
}

template <typename T>
void Animation<T>::rotate(const Point4D &r, double duration) {
    animations.emplace_back(TransformAnimation<T>::rotation, r, duration);
}
template <typename T>
void Animation<T>::rotate(double rx, double ry, double rz, double duration) {
    rotate({rx, ry, rz}, duration);
}

template <typename T>
void Animation<T>::scale(const Point4D &s, double duration) {
    animations.emplace_back(TransformAnimation<T>::scale, s, duration);
}
template <typename T>
void Animation<T>::scale(double sx, double sy, double sz, double duration) {
    scale({sx, sy, sz}, duration);
}
template <typename T>
void Animation<T>::scale(double s, double duration) {
    scale(s, s, s, duration);
}

template <typename T>
void Animation<T>::wait(double duration) {
    animations.emplace_back(TransformAnimation<T>::wait, duration);
}

template <typename T>
void Animation<T>::update(T& obj) {
    if(animations.empty())
        return;
    auto it = animations.begin();

    // If it the first commend is wait we should wait until waiting time is over
    if(it->type() == TransformAnimation<T>::wait)
    {
        if(!it->update(obj))
            animations.erase(it);
        return;
    }
    // Otherwise we iterate over all animation until we meet animations.end() or wait animation
    while(!animations.empty() && (it != animations.end()) && (it->type() != TransformAnimation<T>::wait))
    {
        if(!it->update(obj))
            animations.erase(it++);
        else
            it++;
    }
}


#endif //INC_3DZAVR_ANIMATION_H
