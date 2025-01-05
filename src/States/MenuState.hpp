//
// Created by nelson on 1/4/25.
//

#ifndef MENUSTATE_H
#define MENUSTATE_H
#include <SFML/Graphics.hpp>
#include <slint.h>

#include "GameState.hpp"

class MenuState final : public GameState
{
public:
    MenuState(sf::RenderWindow& window);

    void handleEvent(const sf::Event &event) override;

    void update() override;

    void render(sf::RenderWindow &window) override;

    void onEnter() override;

    void onExit() override;

private:
    std::unique_ptr<sf::Sprite> m_background;
    sf::RenderWindow& m_window;
    slint::ComponentHandle<slint::Component> m_ui;
};

#endif // MENUSTATE_H
