#pragma once

#include <entt/process/process.hpp>
#include <SDL2/SDL.h>
#include "../Components/RenderableComponent.hpp"

class RenderProcess : public entt::process<RenderProcess, Uint64> {
public:
    RenderProcess(entt::registry& registry, SDL_Renderer* renderer)
        : registry_(registry), renderer_(renderer) {}

    void update(float deltaTime) {
        // Clear the renderer
        SDL_SetRenderDrawColor(renderer_, 0, 0, 0, 255);
        SDL_RenderClear(renderer_);

        // Render entities with RenderableComponent
        auto view = registry_.view<RenderableComponent>();
        for (auto entity : view) {
            auto& renderable = view.get<RenderableComponent>(entity);
            SDL_SetRenderDrawColor(renderer_,
                                   renderable.color.r,
                                   renderable.color.g,
                                   renderable.color.b,
                                   renderable.color.a);
            SDL_RenderFillRect(renderer_, &renderable.rect);
        }

        // Present the renderer
        SDL_RenderPresent(renderer_);
    }

private:
    entt::registry& registry_;
    SDL_Renderer* renderer_;
};
