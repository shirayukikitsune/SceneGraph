#pragma once

namespace kitsune::scenegraph::events {

    namespace window {
        enum EventType {
            First = 0,
            Close = First,
            Last
        };
    }

    namespace application {
        enum EventType {
            First = window::Last,
            Quit = First,
            Last
        };
    }

    namespace input {
        enum EventType {
            First = application::Last,
            KeyDown = First,
            KeyUp,
            Last
        };
    }

    constexpr unsigned getEventCount() {
        return input::Last;
    }

}
