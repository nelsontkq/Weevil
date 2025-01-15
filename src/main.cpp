
#include "Game/Game.hpp"
#include "Core/Log.hpp"  // Include the Log header

auto main() -> int
{
    Log::Init();  // Initialize the logging system
    CORE_INFO("Logger initialized!");
    game.run();
    return 0;
}
