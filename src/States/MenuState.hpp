//
// Created by nelson on 1/4/25.
//

#ifndef MENUSTATE_H
#define MENUSTATE_H
#include <SFML/Graphics.hpp>
#include "GameState.hpp"
#include <entt/entt.hpp>
#include "../Components/PositionComponent.hpp"
#include "../Components/RenderComponent.hpp"

class MenuState final : public GameState {
public:
    MenuState(sf::RenderWindow &window, entt::registry &registry);

    void handleEvent(const sf::Event& event) override;

    void update() override;

    void render(sf::RenderWindow &window) override;

    void onEnter() override;

    void onExit() override;

private:
    entt::registry &m_registry;
    sf::RenderWindow &m_window;
};

#endif // MENUSTATE_H
