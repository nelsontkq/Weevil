#include "../Resources/Assets.hpp"
#include "MenuState.hpp"
#include <imgui.h>
#include "../Resources/Logger.hpp"

MenuState::MenuState(sf::RenderWindow &window) : m_window(window)
{
}

void MenuState::onEnter()
{
    // Load state-specific assets
    auto &assets = Assets::getInstance();
    assets.loadDebugMode();
    Logger::getInstance().log("Entered MenuState");
    m_texture = assets.loadTexture("assets/textures/menu_background.png");
    m_background.setTexture(*m_texture);
    m_background.setScale({
        m_window.getSize().x / m_background.getLocalBounds().width,
        m_window.getSize().y / m_background.getLocalBounds().height
    });
}

void MenuState::onExit()
{
}

void MenuState::handleEvent(const sf::Event& event)
{
}

void MenuState::update()
{
}

void MenuState::render(sf::RenderWindow &window)
{
    window.draw(m_background);
}
