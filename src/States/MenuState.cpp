#ifndef MENUSTATE_HPP
#define MENUSTATE_HPP
#include "../Resources/Assets.hpp"
#include "MenuState.hpp"

MenuState::MenuState(sf::RenderWindow& window)
    : m_startGame(false), m_exitGame(false), m_window(window), m_gui(window) {
    if (m_exitGame) {
        m_window.close();
    }
    if (m_startGame) {
        // Implement transitioning to the main game state
        m_startGame = false;
    }
}

void MenuState::onEnter() {
    // Load state-specific assets
    auto &assets = Assets::getInstance();
    assets.loadTexture("menu_background", "assets/textures/menu_background.png");
    m_background = std::make_unique<sf::Sprite>(assets.getTexture("menu_background"));
    // Preload assets for the main menu
    assets.loadFont("main_font", "assets/fonts/VeronaRegular-7Oy8K.ttf");

    // Create a title label
    auto titleLabel = tgui::Label::create("Game of Intrigue");
    titleLabel->setPosition("50%", "20%");
    titleLabel->setHorizontalAlignment(tgui::Label::HorizontalAlignment::Center);
    titleLabel->setTextSize(48);
    m_gui.add(titleLabel);

    // Create "Start Game" button
    auto startButton = tgui::Button::create("Start Game");
    startButton->setPosition("50%", "45%");
    startButton->setSize(200, 50);
    startButton->onPress([this]() { m_startGame = true; });
    m_gui.add(startButton);

    // Create "Exit" button
    auto exitButton = tgui::Button::create("Exit");
    exitButton->setPosition("50%", "55%");
    exitButton->setSize(200, 50);
    exitButton->onPress([this]() { m_exitGame = true; });
    m_gui.add(exitButton);
}

void MenuState::onExit() {
    // Unload assets if necessary (optional optimization)
    auto &assets = Assets::getInstance();
    assets.unloadAll();
    m_gui.removeAllWidgets();
}

void MenuState::handleEvent(const sf::Event &event) {
    m_gui.handleEvent(event);
}

void MenuState::update() {
}

void MenuState::render(sf::RenderWindow &window) {
    window.draw(*m_background);
    m_gui.draw();
}

#endif
