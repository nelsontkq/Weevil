#ifndef MENUSTATE_HPP
#define MENUSTATE_HPP
#include "../Resources/Assets.hpp"
#include "MenuState.hpp"

MenuState::MenuState(sf::RenderWindow &window) : m_window(window) {
}

void MenuState::onEnter() {
    // Load state-specific assets
    auto &assets = Assets::getInstance();
    assets.loadTexture("menu_background", "assets/textures/menu_background.png");
    m_background = std::make_unique<sf::Sprite>(assets.getTexture("menu_background"));
    m_background->setScale({m_window.getSize().x / m_background->getLocalBounds().size.x,
                            m_window.getSize().y / m_background->getLocalBounds().size.y});
    // Preload assets for the main menu
    assets.loadFont("main_font", "assets/fonts/VeronaRegular-7Oy8K.ttf");
}

void MenuState::onExit() {
    // Unload assets if necessary (optional optimization)
    auto &assets = Assets::getInstance();
    assets.unloadAll();
}

void MenuState::handleEvent(const std::optional<sf::Event> event) {
}

void MenuState::update() {
}

void MenuState::render(sf::RenderWindow &window) {
    window.draw(*m_background);
}

#endif
