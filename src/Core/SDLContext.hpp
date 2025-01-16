#pragma once
#include "wvpch.hpp"
#include "../Utils/SDL_Deleter.hpp"
#include "Core/AppSettings.hpp"
namespace wv
{
    // SDLContext class
    class SDLContext
    {
    public:
        SDLContext(AppSettings &settings);
        ~SDLContext();
        SDL_Window *get_window() const;
        SDL_Renderer *get_renderer() const;

    private:
        std::unique_ptr<SDL_Window, SDL_Deleter> window_;
        std::unique_ptr<SDL_Renderer, SDL_Deleter> renderer_;
    };
}