#include "wvpch.hpp"
#include "Game/Game.hpp"

int main()
{
    wv::Log::Init();  // Initialize the logging system
    LOG_INFO("Logger initialized!");

    wv::Game game;
    game.run();
    return 0;
}
