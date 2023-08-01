//
// Created by Ivan Ilin on 26/05/2023.
//

#ifndef SHOOTER_EVENTHANDLER_H
#define SHOOTER_EVENTHANDLER_H

#include "Log.h"

#include <string>
#include <utility>
#include <map>
#include <list>
#include <functional>
#include <memory>

class Event final {
private:
    std::string s_event_name;
public:
    explicit Event(std::string  name) : s_event_name(std::move(name)) {}

    bool operator==(const Event &event) const { return s_event_name == event.s_event_name; }
    bool operator!=(const Event &event) const { return s_event_name != event.s_event_name; }
    bool operator<(const Event &event) const { return s_event_name < event.s_event_name; }

    [[nodiscard]] std::string str() const { return s_event_name; }
};

class Function{};

template<typename Argtype>
class CallBack final : public Function {
private:
    std::function<Argtype> _callBack;
public:
    explicit CallBack(const std::function<Argtype>& callBack): _callBack(callBack) {};

    template<typename... Args>
    void operator()(Args&&... a) {
        _callBack(a...);
    }
};

class EventHandler final {
private:
    std::map<Event, std::list<std::unique_ptr<Function>>> _callBacks;

    static EventHandler *_instance;

    EventHandler() = default;
public:
    EventHandler(const EventHandler &) = delete;

    EventHandler &operator=(EventHandler &) = delete;

    static void init();

    template<typename Argtype, typename... Arguments>
    static void call(const Event& event, Arguments... args) {
        if (_instance == nullptr) {
            return;
        }
        auto functionListIterator = _instance->_callBacks.find(event);

        if(functionListIterator != _instance->_callBacks.end()) {
            auto& functionList = functionListIterator->second;
            for (const auto& function : functionList) {
                auto callBack = *static_cast<CallBack<Argtype>*>(function.get());
                callBack(args...);
            }
        }

        Log::log("EventHandler::call(): event <" + event.str()
                 + "> happened (" + std::to_string(_instance->_callBacks[event].size()) + " listeners)");
    }

    template<typename Argtype>
    static void listen(const Event& event, const std::function<Argtype>& callBack) {
        if (_instance == nullptr) {
            return;
        }
        _instance->_callBacks[event].emplace_back(std::make_unique<CallBack<Argtype>>(callBack));

        Log::log("EventHandler::listen(): added event listener to <" + event.str()
        + "> (" + std::to_string(_instance->_callBacks[event].size()) + " listeners)");
    }
};


#endif //SHOOTER_EVENTHANDLER_H
