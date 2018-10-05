#include "EventHandler.h"

using kitsune::scenegraph::events::EventHandler;

void EventHandler::addHandler(unsigned eventType, EventHandler::eventCallback::function_type &&function) {
    handlers[eventType].push_auto(function);
}

void EventHandler::fireHandler(unsigned eventType, void *data) {
    handlers[eventType](data);
}
