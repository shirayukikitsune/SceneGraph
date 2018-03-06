#pragma once

namespace kitsune {
namespace scenegraph {
namespace events {

namespace window {
    enum EventType {
        First = 0,
        Close = First,
        Last
    };
}

namespace application {
    enum EventType {
        First = 0x100,
        Quit = First,
        Last
    };
}

constexpr unsigned getEventCount() {
    return application::Last;
}

}
}
}
