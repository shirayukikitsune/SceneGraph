#pragma once

#include <exception>
#include <string>
#include <SDL2/SDL.h>

namespace kitsune {
namespace scenegraph {
namespace sdl {
    
    class Graphics {
    public:
        class Exception : public std::exception {
        public:
            explicit Exception(const char *error)
                : error(error) {}

            const char * what() const noexcept {
                return error.c_str();
            }

        private:
            std::string error;
        };

        Graphics(int Width, int Height);
        ~Graphics();

        void setClearColor(float a, float r, float g, float b);
        void clear();
        void present();

        /**
         * @brief Enables or disables vertical syncronization
         *
         * This method will change the current vertical synchronization state.
         * If set to enabled, it will first try using late swap tearing. If that fails, it will fallback to synchronization with vertical retrace.
         */
        void setVerticalSync(bool enabled);

        void setWindowTitle(const char * Title);

        SDL_GLContext getContext() const { return Context; }

    private:
		SDL_Window* Window;
        SDL_GLContext Context;
    };

}
}
}
