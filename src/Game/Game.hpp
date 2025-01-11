//
// Created by nelson on 1/4/25.
//

#ifndef GAME_H
#define GAME_H
#include <memory>
#include <stack>
#include "../States/GameState.hpp"

#include <SFML/Graphics.hpp>
#include <entt/entt.hpp>
#include <imgui-SFML.h>
#include <imgui.h>
#include "../Systems/RenderSystem.hpp"


class Game {
public:
    Game();

    void run();

    void pushState(std::unique_ptr<GameState> state);

    void popState();

    std::unique_ptr<GameState> &getCurrentState();

private:
    void handleEvents();

    void update();

    void render();

    bool m_debugMode = false;
    sf::RenderWindow m_window;
    sf::Clock m_deltaClock;
    sf::View m_view;
    RenderSystem m_renderSystem;
    entt::registry m_registry;
    std::stack<std::unique_ptr<GameState> > m_states;
};


#endif //GAME_H
