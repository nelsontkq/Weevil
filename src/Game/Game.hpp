#ifndef WV_GAME_HPP
#define WV_GAME_HPP

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
        static bool logInitialized_;
        static bool initializeLog();
        AppSettings settings_;
        SDLContext sdlContext_;
        entt::registry registry_;
        entt::basic_scheduler<uint64_t> scheduler_;
        std::unique_ptr<ECS> ecs_;
    };
}

#endif // WV_GAME_HPP
