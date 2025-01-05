#ifndef MENUSTATE_HPP
#define MENUSTATE_HPP
#include "../Resources/Assets.hpp"
#include "../Resources/Assets.hpp"
#include "MenuState.hpp"

MenuState::MenuState(sf::RenderWindow &window)
    : m_startGame(false), m_exitGame(false), m_window(window) {
    m_ui = slint::Component::load_from_path("assets/ui/Menu.slint").unwrap();
}

void MenuState::onEnter() {
    // Load state-specific assets
    auto &assets = Assets::getInstance();
    assets.loadTexture("menu_background", "assets/textures/menu_background.png");
    m_background = std::make_unique<sf::Sprite>(assets.getTexture("menu_background"));
    // Preload assets for the main menu
    assets.loadFont("main_font", "assets/fonts/VeronaRegular-7Oy8K.ttf");

    m_ui->invoke("set_title", "Game of Intrigue");
    m_ui->on("start_game", [&]() { m_startGame = true; });
    m_ui->on("exit_game", [&]() { m_exitGame = true; });
}

void MenuState::onExit() {
    // Unload assets if necessary (optional optimization)
    auto &assets = Assets::getInstance();
    assets.unloadAll();
    m_ui = nullptr;
}

void MenuState::handleEvent(const sf::Event &event) {
    m_ui->handle_event(event);
}

void MenuState::update() {
}

void MenuState::render(sf::RenderWindow &window) {
    window.draw(*m_background);
    m_ui->render();
}

#endif
