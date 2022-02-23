//
// Created by Иван Ильин on 03.10.2021.
//

#include <list>

#include "Timeline.h"
#include "../utils/Log.h"

Timeline *Timeline::_instance = nullptr;

void Timeline::init() {
    delete _instance;
    _instance = new Timeline();

    Log::log("Timeline::init(): animation timeline was initialized");
}

void Timeline::deleteAllAnimations() {
    if (_instance == nullptr) {
        return;
    }

    Log::log("Timeline::deleteAllAnimations(): all " + std::to_string(_instance->_animations.size()) + " list was deleted");
    _instance->_animations.clear();
}

void Timeline::deleteAnimationList(const AnimationListTag &listName) {
    if (_instance == nullptr) {
        return;
    }

    auto it = _instance->_animations.find(listName);

    if(it != _instance->_animations.end()) {
        _instance->_animations.erase(it);
    } else {
        Log::log("Timeline::deleteAnimationList(): list '" + listName.str() + "' does not exist");
    }
}

[[nodiscard]] bool Timeline::isInAnimList(const AnimationListTag &listName) {
    if (_instance == nullptr) {
        return false;
    }

    auto it = _instance->_animations.find(listName);
    if(it != _instance->_animations.end()) {
        return !it->second.empty();
    }

    return false;
}

void Timeline::update() {
    if (_instance == nullptr) {
        return;
    }

    for (auto iter = _instance->_animations.begin(); iter != _instance->_animations.end(); ) {
        if (iter->second.empty()) {
            _instance->_animations.erase(iter++);
            continue;
        }
        auto& animationList = iter->second;
        auto it = animationList.begin();

        // If it the front animation is 'AWait' we should wait until waiting time is over
        if ((it != animationList.end()) && (*it)->isWaitingForFinish()) {
            if (!(*it)->updateState()) {
                animationList.erase(it);
            }
            ++iter;
            continue;
        }

        // Otherwise we iterate over all animation until we meet animations.end() or wait animation
        while (!animationList.empty() && (it != animationList.end()) && (!(*it)->isWaitingForFinish())) {
            if (!(*it)->updateState()) {
                animationList.erase(it++);
            } else {
                ++it;
            }
        }
        ++iter;
    }
}

void Timeline::free() {
    Timeline::deleteAllAnimations();

    delete _instance;
    _instance = nullptr;

    Log::log("Timeline::free(): pointer to 'Timeline' was freed");
}
