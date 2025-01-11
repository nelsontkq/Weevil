#pragma once

#include <SDL2/SDL.h>

struct SDL_Deleter {
    void operator()(SDL_Window* ptr) const {
        if (ptr) {
            SDL_DestroyWindow(ptr);
        }
    }

    void operator()(SDL_Renderer* ptr) const {
        if (ptr) {
            SDL_DestroyRenderer(ptr);
        }
    }

    // Add deleters for other SDL types if needed
    // For example:
    // void operator()(SDL_Texture* ptr) const {
    //     if (ptr) {
    //         SDL_DestroyTexture(ptr);
    //     }
    // }
};
