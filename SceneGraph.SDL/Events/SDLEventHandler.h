#pragma once

#include <Events/EventHandler.h>

#include <SDL2/SDL_events.h>

namespace kitsune::scenegraph::sdl::events {

    class SDLEventHandler : public ::kitsune::scenegraph::events::EventHandler
    {
    public:
        void processEvents() override;

    private:
        void processWindowEvent(SDL_WindowEvent *Event);
    };

}
