#ifndef MENUSTATE_HPP
#define MENUSTATE_HPP
#include "../Resources/Assets.hpp"
#include "../Resources/Assets.hpp"
#include "MenuState.hpp"

MenuState::MenuState(sf::RenderWindow& window)
    : m_startGame(false), m_exitGame(false), m_window(window) {
    m_nkContext = nk_sfml_init(&m_window);
    nk_sfml_font_stash_begin(&m_nkContext);
    nk_sfml_font_stash_end();
}

void MenuState::onEnter() {
    // Load state-specific assets
    auto &assets = Assets::getInstance();
    assets.loadTexture("menu_background", "assets/textures/menu_background.png");
    m_background = std::make_unique<sf::Sprite>(assets.getTexture("menu_background"));
    // Preload assets for the main menu
    assets.loadFont("main_font", "assets/fonts/VeronaRegular-7Oy8K.ttf");

    // Nuklear UI
    if (nk_begin(m_nkContext, "Menu", nk_rect(0, 0, 1920, 1080), NK_WINDOW_BACKGROUND)) {
        nk_layout_row_dynamic(m_nkContext, 50, 1);
        nk_label(m_nkContext, "Game of Intrigue", NK_TEXT_CENTERED);

        nk_layout_row_dynamic(m_nkContext, 50, 1);
        if (nk_button_label(m_nkContext, "Start Game")) {
            m_startGame = true;
        }

        nk_layout_row_dynamic(m_nkContext, 50, 1);
        if (nk_button_label(m_nkContext, "Exit")) {
            m_exitGame = true;
        }
    }
    nk_end(m_nkContext);
}

void MenuState::onExit() {
    // Unload assets if necessary (optional optimization)
    auto &assets = Assets::getInstance();
    assets.unloadAll();
    nk_sfml_shutdown();
}

void MenuState::handleEvent(const sf::Event &event) {
    nk_sfml_handle_event(&event);
}

void MenuState::update() {
}

void MenuState::render(sf::RenderWindow &window) {
    window.draw(*m_background);
    nk_sfml_render(NK_ANTI_ALIASING_ON);
}

#endif
