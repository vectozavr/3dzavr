//
// Created by Ivan Ilin on 26/05/2023.
//

#ifndef SHOOTER_EVENTHANDLER_H
#define SHOOTER_EVENTHANDLER_H

#include <functional>
#include <utility>
#include <memory>
#include <string>
#include <list>
#include <map>

#include <utils/Log.h>

/**
 * @class Event
 * @brief Represents an event within the system.
 *
 * This class encapsulates the details of an event, primarily identified by its name.
 */
class Event final {
private:
    std::string s_event_name; ///< The name of the event.

public:
    /**
     * @brief Constructs an Event with a given name.
     * @param name The name of the event.
     */
    explicit Event(std::string  name) : s_event_name(std::move(name)) {}

    /**
     * @brief Equality comparison operator.
     * @param event The Event to compare with.
     * @return True if the events have the same name, false otherwise.
     */
    bool operator==(const Event &event) const { return s_event_name == event.s_event_name; }

    /**
     * @brief Inequality comparison operator.
     * @param event The Event to compare with.
     * @return True if the events have different names, false otherwise.
     */
    bool operator!=(const Event &event) const { return s_event_name != event.s_event_name; }

    /**
     * @brief Less-than comparison operator.
     * @param event The Event to compare with.
     * @return True if this event's name is lexicographically less than the other's, false otherwise.
     */
    bool operator<(const Event &event) const { return s_event_name < event.s_event_name; }

    /**
     * @brief Gets the event's name.
     * @return The name of the event.
     */
    [[nodiscard]] std::string str() const { return s_event_name; }
};

/**
 * @class Function
 * @brief Base class for callback functions.
 *
 * This class serves as a base for any callback function that might be registered with an event handler.
 */
class Function{};

/**
 * @class CallBack
 * @brief Represents a callback function with a specific argument type.
 *
 * This template class encapsulates a std::function with a specific argument type, allowing for the execution
 * of the callback with the provided arguments.
 */
template<typename Argtype>
class CallBack final : public Function {
private:
    std::function<Argtype> _callBack; ///< The encapsulated callback function.
public:

    /**
     * @brief Constructs a CallBack with a given std::function.
     * @param callBack The std::function to encapsulate.
     */
    explicit CallBack(const std::function<Argtype>& callBack): _callBack(callBack) {};

    /**
     * @brief Invokes the callback function with the provided arguments.
     * @param a The arguments to pass to the callback function.
     */
    template<typename... Args>
    void operator()(Args&&... a) {
        _callBack(a...);
    }
};

/**
 * @class EventHandler
 * @brief Manages event callbacks within the system.
 *
 * This singleton class is responsible for managing event callbacks. It allows registering callbacks for specific events
 * and calling them when the event occurs. The EventHandler ensures that all callbacks associated with an event are called
 * whenever the event is triggered.
 *
 * Usage:
 * - Use `EventHandler::listen` to register a callback for a specific event.
 * - Use `EventHandler::call` to trigger an event and call all registered callbacks.
 *
 * Note:
 * - This class is designed as a singleton to ensure a centralized management of events.
 * - Copying and assignment are deleted to enforce singleton behavior.
 */
class EventHandler final {
private:
    std::map<Event, std::list<std::unique_ptr<Function>>> _callBacks; ///< Stores callbacks for each event.

    static EventHandler *_instance; ///< Singleton instance of the EventHandler.

    EventHandler() = default; ///< Private constructor for singleton pattern.

public:
    EventHandler(const EventHandler &) = delete; ///< Deleted copy constructor.
    EventHandler &operator=(EventHandler &) = delete; ///< Deleted assignment operator.

    /**
     * @brief Initializes the singleton instance of the EventHandler.
     */
    static void init();

    /**
     * @brief Calls all callbacks registered for a specific event.
     * @param event The event to trigger.
     * @param args The arguments to pass to the callbacks.
     *
     * This function finds all callbacks registered for the specified event and calls them with the provided arguments.
     * It logs the event occurrence and the number of listeners notified.
     */
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

    /**
     * @brief Registers a callback for a specific event.
     * @param event The event to listen for.
     * @param callBack The callback function to register.
     *
     * This function adds the provided callback to the list of callbacks for the specified event. It logs the addition
     * of the listener to the event.
     */
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
