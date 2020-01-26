#include "Graphics.h"

#include <glad/glad.h>
#include <SDL2/SDL.h>

using kitsune::scenegraph::sdl::Graphics;

Graphics::Graphics(int Width, int Height)
{
    SDL_GL_LoadLibrary(nullptr);

    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
    // Set OpenGL Core profile
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    // Set a context flag to force only recent OpenGL functions
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG | SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
    // Enable double buffer and depth buffer
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    // Set min version as OpenGL 4.0 Core
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

    Window = SDL_CreateWindow("", 0, 0, Width, Height, SDL_WINDOW_OPENGL | SDL_WINDOW_BORDERLESS);
    if (!Window) {
        throw Graphics::Exception(SDL_GetError());
    }

    Context = SDL_GL_CreateContext(Window);
    if (!Context) {
        throw Graphics::Exception(SDL_GetError());
    }

    // Initialize glad
    if (!gladLoadGLLoader(SDL_GL_GetProcAddress)) {
        throw Graphics::Exception("Failed to initialize GLAD");
    }

    // set a black clear color as default
    setClearColor(1.0f, 0.0f, 0.0f, 0.0f);

    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
}

Graphics::~Graphics()
{
    if (Context) {
        SDL_GL_DeleteContext(Context);
    }

    if (Window) {
        SDL_DestroyWindow(Window);
    }
}

void Graphics::setClearColor(float a, float r, float g, float b)
{
    glClearColor(r, g, b, a);
}

void Graphics::clear()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Graphics::present()
{
    SDL_GL_SwapWindow(Window);
}

void Graphics::setVerticalSync(bool enabled)
{
    if (enabled) {
        if (SDL_GL_SetSwapInterval(-1) == -1) {
            SDL_GL_SetSwapInterval(1);
        }
    } else {
        SDL_GL_SetSwapInterval(0);
    }
}

void Graphics::setWindowTitle(const char * Title)
{
    SDL_SetWindowTitle(Window, Title);
}
