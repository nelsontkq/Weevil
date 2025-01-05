//
// Created by nelson on 1/4/25.
//

#ifndef MENUSTATE_H
#define MENUSTATE_H
#include <SFML/Graphics.hpp>

#include "GameState.hpp"

class MenuState final : public GameState
{
public:
    MenuState();

    void handleEvent(const sf::Event &event) override;

    void update() override;

    void render(sf::RenderWindow &window) override;

    void onEnter() override;

    void onExit() override;

private:
    std::unique_ptr<sf::Sprite> m_background;
    std::unique_ptr<sf::Text> m_titleText;
    std::unique_ptr<sf::Text> m_startGameText;
    std::unique_ptr<sf::Text> m_exitText;

    bool m_startGame;
    bool m_exitGame;
};

#endif // MENUSTATE_H
