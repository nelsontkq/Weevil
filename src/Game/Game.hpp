#pragma once

#include "wvpch.hpp"
#include "../Utils/SDL_Deleter.hpp"
#include "../ECS.hpp"
#include "../Core/AppSettings.hpp"
#include "../Core/SDLContext.hpp"

namespace wv
{

    class Game
    {
    public:
        Game();
        ~Game();

        void run();


    private:
        AppSettings settings_;
        SDLContext sdlContext_;
        entt::registry registry_;
        entt::basic_scheduler<uint64_t> scheduler_;
        std::unique_ptr<ECS> ecs_;
    };
}