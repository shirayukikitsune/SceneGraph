#pragma once

#include <Events/EventHandler.h>

#include <SDL2/SDL_events.h>

namespace kitsune {
namespace scenegraph {
namespace sdl {
namespace events {

    class SDLEventHandler : public ::kitsune::scenegraph::events::EventHandler
    {
    public:
        virtual void processEvents() override;

    private:
        void processWindowEvent(SDL_WindowEvent *Event);
    };

}
}
}
}
