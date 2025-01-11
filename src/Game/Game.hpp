#pragma once

#include <entt/entt.hpp>

class Game {
public:
    Game();
    ~Game();

    void run();

private:
    entt::registry registry_;
};
