#include "SDLEventHandler.h"

#include <SDL2/SDL_events.h>

using kitsune::scenegraph::sdl::events::SDLEventHandler;
namespace sge = kitsune::scenegraph::events;

void SDLEventHandler::processEvents()
{
    SDL_Event Event;

    while (SDL_PollEvent(&Event)) {
        switch (Event.type) {
        case SDL_QUIT:
            fireHandler(sge::application::Quit, nullptr);
            break;
        case SDL_WINDOWEVENT:
            processWindowEvent(&Event.window);
            break;
        default:
            break;
        }
    }
}

void SDLEventHandler::processWindowEvent(SDL_WindowEvent *Event)
{
    switch (Event->event) {
    case SDL_WINDOWEVENT_CLOSE:
        fireHandler(sge::window::Close, &Event->windowID);
        break;
    default:
        break;
    }
}
