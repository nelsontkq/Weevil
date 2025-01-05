//
// Created by nelson on 1/4/25.
//

#ifndef GAME_H
#define GAME_H
#include <memory>
#include <stack>
#include <SFML/Graphics.hpp>

#include "../States/GameState.hpp"


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

    sf::RenderWindow m_window;
    std::stack<std::unique_ptr<GameState> > m_states;
};


#endif //GAME_H
