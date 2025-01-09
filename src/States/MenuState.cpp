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
    m_background = std::make_unique<sf::Sprite>(*m_texture);
    m_background->setScale({m_window.getSize().x / m_background->getLocalBounds().size.x,
                            m_window.getSize().y / m_background->getLocalBounds().size.y});
}

void MenuState::onExit()
{
}

void MenuState::handleEvent(const sf::Event &event)
{
}

void MenuState::update()
{
}

void MenuState::render(sf::RenderWindow &window)
{
    window.draw(*m_background);
}
