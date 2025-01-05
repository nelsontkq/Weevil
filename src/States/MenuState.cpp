#ifndef MENUSTATE_HPP
#define MENUSTATE_HPP
#include "../Resources/Assets.hpp"
#include "MenuState.hpp"

MenuState::MenuState()
    : m_startGame(false), m_exitGame(false) {
}

void MenuState::onEnter() {
    // Load state-specific assets
    auto &assets = Assets::getInstance();
    assets.loadTexture("menu_background", "assets/textures/menu_background.png");
    m_background = std::make_unique<sf::Sprite>(assets.getTexture("menu_background"));
    // Preload assets for the main menu
    assets.loadFont("main_font", "assets/fonts/VeronaRegular-7Oy8K.ttf");

    // Prepare text objects
    auto &m_font = assets.getFont("main_font");

    m_titleText = std::make_unique<sf::Text>(m_font);
    m_titleText->setString("Game of Intrigue");
    m_titleText->setCharacterSize(48);
    m_titleText->setFillColor(sf::Color::White);
    m_titleText->setPosition({200, 100});

    m_startGameText = std::make_unique<sf::Text>(m_font);
    m_startGameText->setString("Start Game");
    m_startGameText->setCharacterSize(36);
    m_startGameText->setFillColor(sf::Color::White);
    m_startGameText->setPosition({250, 250});

    m_exitText = std::make_unique<sf::Text>(m_font);
    m_exitText->setString("Exit");
    m_exitText->setCharacterSize(36);
    m_exitText->setFillColor(sf::Color::White);
    m_exitText->setPosition({250, 350});
}

void MenuState::onExit() {
    // Unload assets if necessary (optional optimization)
    auto &assets = Assets::getInstance();
    assets.unloadAll();
    m_background.reset();
}

void MenuState::handleEvent(const sf::Event &event) {
    if (const auto key = event.getIf<sf::Event::KeyPressed>()) {
        if (key->code == sf::Keyboard::Key::Enter) {
            // "Enter" -> Start Game
            m_startGame = true;
        }
        if (key->code == sf::Keyboard::Key::Escape) {
            // "Escape" -> Exit Game
            m_exitGame = true;
        }
    }
}

void MenuState::update() {
}

void MenuState::render(sf::RenderWindow &window) {
    window.draw(*m_background);
    window.draw(*m_titleText);
    window.draw(*m_startGameText);
    window.draw(*m_exitText);
}

#endif
