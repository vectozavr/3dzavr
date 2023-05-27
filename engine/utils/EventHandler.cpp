//
// Created by Ivan Ilin on 26/05/2023.
//

#include "EventHandler.h"

EventHandler *EventHandler::_instance = nullptr;

void EventHandler::init() {
    delete _instance;
    _instance = new EventHandler();

    Log::log("EventHandler::init(): event handler was initialized");
}
