
#include "SDLContext.hpp"
using namespace wv;

SDLContext::SDLContext(AppSettings &settings)
{
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        throw std::runtime_error("Failed to initialize SDL: " + std::string(SDL_GetError()));
    }

    // Create SDL window using smart pointer
    Uint32 flags = SDL_WINDOW_SHOWN;
    if (settings.fullscreen)
    {
        flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
    }
    window_.reset(SDL_CreateWindow(settings.title.c_str(),
                                   SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                   settings.width, settings.height,
                                   flags));
    if (!window_)
    {
        throw std::runtime_error("Failed to create SDL Window: " + std::string(SDL_GetError()));
    }
    renderer_.reset(SDL_CreateRenderer(window_.get(), -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC));
    if (!renderer_)
    {
        throw std::runtime_error("Failed to create SDL Renderer: " + std::string(SDL_GetError()));
    }
}

SDLContext::~SDLContext()
{
    SDL_Quit();
}

SDL_Window *SDLContext::get_window() const
{
    return window_.get();
}

SDL_Renderer *SDLContext::get_renderer() const
{
    return renderer_.get();
}