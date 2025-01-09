//
// Created by nelson on 1/4/25.
//

#ifndef MENUSTATE_H
#define MENUSTATE_H
#include <SFML/Graphics.hpp>
#include "GameState.hpp"

class MenuState final : public GameState {
public:
    MenuState(sf::RenderWindow &window);

    void handleEvent(const std::optional<sf::Event> &event) override;

    void update() override;

    void render(sf::RenderWindow &window) override;

    void onEnter() override;

    void onExit() override;

private:
    std::unique_ptr<sf::Sprite> m_background;
    std::shared_ptr<sf::Texture> m_texture;
    sf::RenderWindow &m_window;
};

#endif // MENUSTATE_H
