//
// Created by nelson on 1/4/25.
//

#ifndef MENUSTATE_H
#define MENUSTATE_H
#include <SFML/Graphics.hpp>
#include <nuklear.h>
#include <nuklear_sfml_gl2.h>

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
    std::unique_ptr<nk_context> m_nkContext;

    bool m_startGame;
    bool m_exitGame;
};

#endif // MENUSTATE_H
