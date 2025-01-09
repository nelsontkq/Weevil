//
// Created by nelson on 1/5/25.
//

#ifndef MAINGAMESTATE_HPP
#define MAINGAMESTATE_HPP
#include "GameState.hpp"


class MainGameState : public GameState {
public:
    void handleEvent(const sf::Event &event) override;

    void update() override;

    void render(sf::RenderWindow &window) override;

    void onEnter() override;

    void onExit() override;
};



#endif //MAINGAMESTATE_HPP
