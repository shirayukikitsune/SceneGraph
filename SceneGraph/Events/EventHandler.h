#pragma once

#include "EventType.h"

#include <Base/Callback.h>
#include <array>

namespace kitsune {
namespace scenegraph {
namespace events {

    class EventHandler
    {
    public:
        typedef auto_callback<void(void*)> eventCallback;

        virtual void processEvents() = 0;

        void addHandler(unsigned eventType, eventCallback::function_type && function);

    protected:
        void fireHandler(unsigned eventType, void * data);

    private:
        std::array<eventCallback, events::getEventCount()> handlers;
    };

}
}
}
