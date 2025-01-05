//
// Created by nelson on 1/4/25.
//

#ifndef MENUSTATE_H
#define MENUSTATE_H
#include <SFML/Graphics.hpp>

#include "GameState.hpp"


class MenuState final : public GameState {
public:
    MenuState();

    void handleEvent(const std::optional<sf::Event> &event) override;

    void update() override;

    void render(sf::RenderWindow &window) override;
};


#endif //MENUSTATE_H
